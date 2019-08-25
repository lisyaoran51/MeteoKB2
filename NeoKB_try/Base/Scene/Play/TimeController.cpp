#include "TimeController.h"

using namespace Base::Scenes::Play;
using namespace Framework::Timing::SpeedAdjusters;
using namespace Framework::Timing;

int TimeController::load()
{
	return 0;
}

int TimeController::update()
{
	if (isPaused)
		return 0;

	if (isTriggeredPause) {
		Pause();
		isTriggeredPause = false;
	}

	if (isTriggeredResume) {
		Resume();
		isTriggeredResume = false;
	}
	
	if (isTriggeredSeekingTime) {
		JumpTo(targetSeekTime);
		isTriggeredSeekingTime = false;
	}

	if (isSeekingTime) {
		JumpTo(targetSeekTime);
	}


	return 0;
}



int TimeController::SetDecoupledInterpolatingFramedClock(DecoupledInterpolatingFramedClock * dInterpolatingFramedClock)
{
	controllableClock = dInterpolatingFramedClock;
	return 0;
}

int TimeController::SetAdjustableClock(AdjustableClock * aClock)
{
	audioClock = aClock;
	return 0;
}

int TimeController::SetSpeedAdjuster(SpeedAdjuster * sAdjuster)
{
	// TODO: 做一個thread safe lock再刪
	delete speedAdjuster;
	speedAdjuster = sAdjuster;
	return 0;
}

int TimeController::JumpTo(double seekTime)
{
	audioClock->Seek(seekTime);
	controllableClock->Seek(seekTime);
	return 0;
}

int TimeController::JumpToWithSpeedAdjust(double seekTime)
{
	JumpTo(speedAdjuster->GenerateTime(GetClock()->GetElapsedFrameTime()));
	return 0;
}

int TimeController::Pause()
{
	audioClock->Stop();

	controllableClock->SetIsProcessSourceClockFrames(false);

	isPaused = true;

	return 0;
}

int TimeController::Resume()
{
	if (rate == 1) {
		audioClock->Seek(controllableClock->GetCurrentTime());
		audioClock->Start();
	}
	controllableClock->SetIsProcessSourceClockFrames(true);
	isPaused = false;
	return 0;
}

int TimeController::SetRate(double r)
{
	if (r == 0)
		throw invalid_argument("TimeController::SetRate() : 0 is not available for rate.");
	if (r != 1)
		audioClock->Stop();
	rate = r;
	audioClock->SetRate(r);
	controllableClock->SetRate(r);
	return 0;
}

int TimeController::GetRate()
{
	return rate;
}
