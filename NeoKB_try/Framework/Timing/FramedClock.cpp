#include "FramedClock.h"

#include "StopwatchClock.h"
#include <stdexcept>


using namespace Framework::Timing;
using namespace std;


FramedClock::FramedClock(Clock * s)
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

	// TODO: �p��averageFrameTime�MframesPerSecond

	if (!isStarted) 
		start();
	

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
	return source->SetRate(r);
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
	return source->SetIsRunning(value);
}

bool FramedClock::GetIsRunning()
{
	return source->GetIsRunning();
}

double FramedClock::GetElapsedFrameTime()
{
	return currentTime - lastFrameTime;
}

double FramedClock::GetAverageFrameTime()
{
	// TODO: �����g
	return averageFrameTime;
}

double FramedClock::GetFramesPerSecond()
{
	// TODO: �����g
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
	
	AdjustableClock* adjustableClock = dynamic_cast<AdjustableClock*>(source);
	if (adjustableClock != nullptr)
		adjustableClock->Seek(cTime);
	else {

		throw runtime_error("int FramedClock::setCurrentTime() : error. source clock is not adjustable.");
	}
	lastFrameTime = currentTime;
	currentTime = cTime;
	return 0;
}

int FramedClock::start()
{
	currentTime = source->GetCurrentTime();
	isStarted = true;
	return 0;
}
