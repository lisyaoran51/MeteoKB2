#include "DecoupledInterpolatingFramedClock.h"

#include "../../Util/MtoType.h"


using namespace Framework::Timing;

DecoupledInterpolatingFramedClock::DecoupledInterpolatingFramedClock(): InterpolatingFramedClock()
{
	decoupledClock = new FramedClock(decoupledStopwatchClock = new StopwatchClock());
}

double DecoupledInterpolatingFramedClock::GetCurrentTime()
{
	// 用分離時鐘嗎? 拿分離時鐘時間 : 拿原始時鐘的時間(可能會停掉)
	return getIsUseDecoupledClock() ? decoupledClock->GetCurrentTime() : InterpolatingFramedClock::GetCurrentTime();
}

int DecoupledInterpolatingFramedClock::SetRate(double r)
{
	if (source != nullptr)
		return source->SetRate(r);
	return -1;
}

double DecoupledInterpolatingFramedClock::GetRate()
{
	if (source != nullptr)
		return source->GetRate();
	return 1;
}

bool DecoupledInterpolatingFramedClock::GetIsRunning()
{
	return getIsUseDecoupledClock() ? decoupledClock->GetIsRunning() : InterpolatingFramedClock::GetIsRunning();
}

int DecoupledInterpolatingFramedClock::ChangeSource(Clock * s)
{
	if (s == nullptr)
		return 0;

	if (dynamic_cast<AdjustableClock*>(s))
		dynamic_cast<AdjustableClock*>(s)->Seek(GetCurrentTime());

	source = s;
	framedSource = dynamic_cast<FramedClock*>(s) != nullptr ? dynamic_cast<FrameBasedClock*>(s) : new FramedClock(s);
	return 0;
}

double DecoupledInterpolatingFramedClock::GetElapsedFrameTime()
{
	return getIsUseDecoupledClock() ? decoupledClock->GetElapsedFrameTime() : InterpolatingFramedClock::GetElapsedFrameTime();
}

int DecoupledInterpolatingFramedClock::ProcessFrame()
{
	InterpolatingFramedClock::ProcessFrame();

	decoupledStopwatchClock->SetRate(getAdjustableSource()->GetRate());
	decoupledClock->ProcessFrame();


	bool sourceRunning = false;
	if (source != nullptr)
		sourceRunning = source->GetIsRunning();

	/* 讓分離的時鐘一直跟著現在的時間 */
	if (isCoupled || sourceRunning) {
		if (sourceRunning)
			decoupledStopwatchClock->Start();
		else
			decoupledStopwatchClock->Stop();
		decoupledStopwatchClock->Seek(GetCurrentTime());
	}
	else {
		if (decoupledClock->GetIsRunning()) {
			//if we're running but our source isn't, we should try a seek to see if it's capable to switch to it for the current value.
			Start();
		}
	}



	// 還沒寫
	return 0;
}

int DecoupledInterpolatingFramedClock::Reset()
{
	isCoupled = true;

	if (getAdjustableSource() != nullptr)
		getAdjustableSource()->Reset();
	decoupledStopwatchClock->Reset();
	return 0;
}

int DecoupledInterpolatingFramedClock::Start()
{
	if(getAdjustableSource() != nullptr)
	if(!getAdjustableSource()->GetIsRunning()){
		if (isCoupled || getAdjustableSource()->Seek(GetCurrentTime()) == RETURN_AVAILABLE)
			//only start the source clock if our time values match.
			//this handles the case where we seeked to an unsupported value and the source clock is out of sync.
			getAdjustableSource()->Start();
	}
	decoupledStopwatchClock->Start();
	return 0;
}

int DecoupledInterpolatingFramedClock::Stop()
{
	if (getAdjustableSource() != nullptr)
		getAdjustableSource()->Stop();
	decoupledStopwatchClock->Stop();
	return 0;
}

bool DecoupledInterpolatingFramedClock::Seek(double position)
{
	bool success = false; // 之後要回來看看初始直是不是false
	if (getAdjustableSource() != nullptr)
		success = getAdjustableSource()->Seek(position) == true;

	if (isCoupled)
	{
		decoupledStopwatchClock->Seek(getAdjustableSource() != nullptr ? getAdjustableSource()->GetCurrentTime() : position);
		return success;
	}

	if (!success)
		//if we failed to seek then stop the source and use decoupled mode.
		if (getAdjustableSource() != nullptr)
			getAdjustableSource()->Stop();

	return decoupledStopwatchClock->Seek(position);
}

int DecoupledInterpolatingFramedClock::ResetSpeedAdjustments()
{
	InterpolatingFramedClock::SetRate(1);
	return 0;
}

int DecoupledInterpolatingFramedClock::SetIsCoupled(bool value)
{
	isCoupled = value;
	return 0;
}

bool DecoupledInterpolatingFramedClock::GetIsCoupled()
{
	return isCoupled;
}

AdjustableClock * DecoupledInterpolatingFramedClock::getAdjustableSource()
{
	if (source == nullptr)
		return nullptr;
	return dynamic_cast<AdjustableClock*>(source);
}

bool DecoupledInterpolatingFramedClock::getIsUseDecoupledClock()
{
	return (framedSource == nullptr || !isCoupled) && !framedSource->GetIsRunning();
}
