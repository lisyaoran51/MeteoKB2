#include "FramedClock.h"

#include "StopwatchClock.h"

using namespace Framework::Timing;

FramedClock::FramedClock(Clock * s = nullptr)
{
	currentTime = s->GetCurrentTime();
	if (s != nullptr) {
		source = s;
	}
	else {
		source = new StopwatchClock();
	}
	
	isProcessSourceClockFrames = true;
}

int FramedClock::ProcessFrame()
{
	if (isProcessSourceClockFrames) {
		FrameBasedClock* sourceAsFrameBased = dynamic_cast<FrameBasedClock*>(source);
		if (sourceAsFrameBased)
			sourceAsFrameBased->ProcessFrame();
	}

	// TODO: ­pºâaverageFrameTime©MframesPerSecond

	lastFrameTime = currentTime;
	currentTime = source->GetCurrentTime();

	return 0;
}

Clock * FramedClock::GetSource()
{
	return source;
}

int FramedClock::SetRate(double r)
{
	rate = r;
	return 0;
}

double FramedClock::GetRate()
{
	return source->GetRate();
}

double FramedClock::GetCurrentTime()
{
	return currentTime;
}

int FramedClock::SetIsRunning(bool value)
{
	isRunning = value;
	return 0;
}

bool FramedClock::GetIsRunning()
{
	return isRunning;
}

double FramedClock::GetElapsedFrameTime()
{
	return currentTime - lastFrameTime;
}

double FramedClock::GetAverageFrameTime()
{
	return averageFrameTime;
}

double FramedClock::GetFramesPerSecond()
{
	return framesPerSecond;
}

FrameTimeInfo * FramedClock::GetFrameTimeInfo()
{
	FrameTimeInfo* timeInfo = new FrameTimeInfo();
	timeInfo->Current = GetCurrentTime();
	timeInfo->Elapsed = GetElapsedFrameTime();
	return timeInfo;
}

int FramedClock::SetIsProcessSourceClockFrames(bool value)
{
	isProcessSourceClockFrames = value;
	return 0;
}

bool FramedClock::GetIsProcessSourceClockFrames()
{
	return isProcessSourceClockFrames;
}

int FramedClock::setCurrentTime(double cTime)
{
	currentTime = cTime;
	return 0;
}
