#include "DecoupledInterpolatingFramedClock.h"

#include "../../Util/MtoType.h"
#include "../../Util/Log.h"


using namespace Framework::Timing;
using namespace Util;


DecoupledInterpolatingFramedClock::DecoupledInterpolatingFramedClock(): InterpolatingFramedClock()
{
	LOG(LogLevel::Debug) << "DecoupledInterpolatingFramedClock::DecoupledInterpolatingFramedClock() : address = [" << this << "].";
	decoupledClock = new FramedClock(decoupledStopwatchClock = new StopwatchClock());
}

double DecoupledInterpolatingFramedClock::GetCurrentTime()
{
	LOG(LogLevel::Depricated) << "DecoupledInterpolatingFramedClock::GetCurrentTime() : get decoupled clock? [" << getIsUseDecoupledClock() << "].";
	// 用分離時鐘嗎? 拿分離時鐘時間 : 拿原始時鐘的時間(可能會停掉)
	return getIsUseDecoupledClock() ? decoupledClock->GetCurrentTime() : InterpolatingFramedClock::GetCurrentTime();
}

int DecoupledInterpolatingFramedClock::SetRate(double r)
{
	decoupledClock->SetRate(r);
	InterpolatingFramedClock::SetRate(r);
	
	return 0;
}

int DecoupledInterpolatingFramedClock::SetIsRunning(bool value)
{
	decoupledClock->SetIsRunning(value);
	if (framedSource != nullptr)
	if (getAdjustableSource()->Seek(GetCurrentTime()) == RETURN_AVAILABLE) {
		InterpolatingFramedClock::SetIsRunning(value);
	}

	return 0;
}

bool DecoupledInterpolatingFramedClock::GetIsRunning()
{
	return getIsUseDecoupledClock() ? decoupledClock->GetIsRunning() : InterpolatingFramedClock::GetIsRunning();
}

int DecoupledInterpolatingFramedClock::ChangeSource(Clock * s)
{
	if (s == nullptr)
		return 0;

	LOG(LogLevel::Debug) << "DecoupledInterpolatingFramedClock::ChangeSource : changing.";

	if (dynamic_cast<AdjustableClock*>(s))
		dynamic_cast<AdjustableClock*>(s)->Seek(GetCurrentTime());

	LOG(LogLevel::Debug) << "DecoupledInterpolatingFramedClock::ChangeSource : get framed source.";
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
	LOG(LogLevel::Finest) << "DecoupledInterpolatingFramedClock::ProcessFrame() : " << this;
	InterpolatingFramedClock::ProcessFrame();
	LOG(LogLevel::Finest) << "DecoupledInterpolatingFramedClock::ProcessFrame() : process decoupled clock";
	decoupledStopwatchClock->SetRate(InterpolatingFramedClock::GetRate());
	decoupledClock->ProcessFrame();


	bool sourceRunning = InterpolatingFramedClock::GetIsRunning();
	LOG(LogLevel::Finest) << "DecoupledInterpolatingFramedClock::ProcessFrame() : determine if decoupled clock start. coupled = ["<< isCoupled << "], source running = [" << (sourceRunning ? 1 : 0) << "]";
	// ***讓分離的時鐘一直跟著現在的時間***
	// 只要原時鐘有在跑，分離時鐘就一定要跟著原時鐘的時間。但如果原時鐘沒在跑的話， 就要看有沒有couple，
	// 有couple的狀況，分離時鐘就必須停下來保持跟原時鐘一樣的時間。
	// 沒有couple的狀況，分離時鐘就擺脫原時鐘，繼續跑下去
	if (isCoupled || sourceRunning) { // 有綁在一起，或者是原時鐘正在跑，都一定要跟著
		if (sourceRunning)
			decoupledStopwatchClock->Start();
		else
			decoupledStopwatchClock->Stop();
		decoupledStopwatchClock->Seek(GetCurrentTime());
	}
	else {
		if (decoupledClock->GetIsRunning()) {
			//if we're running but our source isn't, we should try a seek to see if it's capable to switch to it for the current value.
			// 這邊是打開遊戲音樂的地方
			LOG(LogLevel::Info) << "DecoupledInterpolatingFramedClock::ProcessFrame() : music start";
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
		LOG(LogLevel::Info) << "DecoupledInterpolatingFramedClock::Start() : check if coupled [" << isCoupled << "] and seek time [" << GetCurrentTime() << "].";
		if (isCoupled || getAdjustableSource()->Seek(GetCurrentTime())) {
			//only start the source clock if our time values match.
			//this handles the case where we seeked to an unsupported value and the source clock is out of sync.
			LOG(LogLevel::Debug) << "DecoupledInterpolatingFramedClock::Start() : start audio clock";
			getAdjustableSource()->Start();
		}
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
	InterpolatingFramedClock::SetRate(1.0);
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
	if (framedSource == nullptr)
		return !isCoupled;
	else
		return !isCoupled && !framedSource->GetIsRunning();
}
