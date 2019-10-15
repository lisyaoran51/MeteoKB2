#include "TimeController.h"

using namespace Games::Scenes::Play;
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



int TimeController::SetAudioClock(AdjustableClock * dInterpolatingFramedClock)
{
	
	audioClock = dInterpolatingFramedClock;
	return 0;
}

int TimeController::SetControllableClock(DecoupledInterpolatingFramedClock * cClock)
{
	controllableClock = cClock;
	return 0;
}

int TimeController::SetFramedClock(FramedClock * fClock)
{
	framedClock = fClock;
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
