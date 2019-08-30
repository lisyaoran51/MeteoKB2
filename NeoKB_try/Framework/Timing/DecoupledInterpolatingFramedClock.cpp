#include "DecoupledInterpolatingFramedClock.h"

#include "../../Util/MtoType.h"


using namespace Framework::Timing;

DecoupledInterpolatingFramedClock::DecoupledInterpolatingFramedClock(): InterpolatingFramedClock()
{
	decoupledClock = new FramedClock(decoupledStopwatchClock = new StopwatchClock());
	isCoupled = true;
	adjustableSource = dynamic_cast<AdjustableClock*>(source);
}

double DecoupledInterpolatingFramedClock::GetCurrentTime()
{

	return getIsUseDecoupledClock() ? decoupledClock->GetCurrentTime() : InterpolatingFramedClock::GetCurrentTime();
}

int DecoupledInterpolatingFramedClock::SetRate(double r)
{
	return adjustableSource->SetRate(r);
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
	return 0;
}

int DecoupledInterpolatingFramedClock::Reset()
{
	isCoupled = true;

	if (adjustableSource != nullptr)
		adjustableSource->Reset();
	decoupledStopwatchClock->Reset();
	return 0;
}

int DecoupledInterpolatingFramedClock::Start()
{
	if(adjustableSource != nullptr)
	if(!adjustableSource->GetIsRunning()){
		if (isCoupled || adjustableSource->Seek(GetCurrentTime()) == RETURN_AVAILABLE)
			//only start the source clock if our time values match.
			//this handles the case where we seeked to an unsupported value and the source clock is out of sync.
			adjustableSource->Start();
	}
	decoupledStopwatchClock->Start();
	return 0;
}

int DecoupledInterpolatingFramedClock::Stop()
{
	if (adjustableSource != nullptr)
		adjustableSource->Stop();
	decoupledStopwatchClock->Stop();
	return 0;
}

bool DecoupledInterpolatingFramedClock::Seek(double position)
{
	bool success = false; // 之後要回來看看初始直是不是false
	if (adjustableSource != nullptr)
		success = adjustableSource->Seek(position) == true;

	if (isCoupled)
	{
		decoupledStopwatchClock->Seek(adjustableSource != nullptr ? adjustableSource->GetCurrentTime() : position);
		return success;
	}

	if (!success)
		//if we failed to seek then stop the source and use decoupled mode.
		if (adjustableSource != nullptr)
			adjustableSource->Stop();

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

bool DecoupledInterpolatingFramedClock::getIsUseDecoupledClock()
{
	return framedSource == nullptr || !isCoupled && !framedSource->GetIsRunning();
}
