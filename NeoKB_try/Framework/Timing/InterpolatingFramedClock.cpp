#include "InterpolatingFramedClock.h"

#include "StopwatchClock.h"

using namespace Framework::Timing;


InterpolatingFramedClock::InterpolatingFramedClock(Clock * s)
{
	interpolateClock = new FramedClock(new StopwatchClock());
	ChangeSource(s);

	allowableErrorMilliseconds = 1000.0 / 60 * 2;
}

int InterpolatingFramedClock::ProcessFrame()
{
	if (framedSource == nullptr)
		return 0;

	interpolateClock->ProcessFrame();
	framedSource->ProcessFrame();

	SetIsRunning(sourceIsRunning = framedSource->GetIsRunning());

	lastInterpolatedTime = GetCurrentTime();

	if (!framedSource->GetIsRunning())
		return 0;

	currentInterpolatedTime += interpolateClock->GetElapsedFrameTime() * GetRate();

	if (fabs(framedSource->GetCurrentTime() - currentInterpolatedTime) > allowableErrorMilliseconds)
	{
		//if we've exceeded the allowable error, we should use the source clock's time value.
		currentInterpolatedTime = framedSource->GetCurrentTime();
	}
	else
	{
		//if we differ from the elapsed time of the source, let's adjust for the difference.
		currentInterpolatedTime += (framedSource->GetCurrentTime() - currentInterpolatedTime) / 8;
	}

	return 0;
}

int InterpolatingFramedClock::ChangeSource(Clock * s)
{
	if (s != nullptr)
	{
		source = s;
		framedSource = dynamic_cast<FrameBasedClock*>(s) != nullptr ? dynamic_cast<FrameBasedClock*>(s) : new FramedClock(s);
	}

	lastInterpolatedTime = 0;
	currentInterpolatedTime = 0;
	return 0;
}

double InterpolatingFramedClock::GetCurrentTime()
{
	return sourceIsRunning ? currentInterpolatedTime : framedSource->GetCurrentTime();
}

int InterpolatingFramedClock::SetRate(double r)
{
	rate = r;
	return 0;
}

double InterpolatingFramedClock::GetRate()
{
	return framedSource->GetRate();
}

int InterpolatingFramedClock::SetIsRunning(bool value)
{
	framedSource->SetIsRunning(value);
	return 0;
}

bool InterpolatingFramedClock::GetIsRunning()
{
	return sourceIsRunning;
}

double InterpolatingFramedClock::GetDrift()
{
	return GetCurrentTime() - framedSource->GetCurrentTime();
}

double InterpolatingFramedClock::GetElapsedFrameTime()
{
	return currentInterpolatedTime - lastInterpolatedTime;;
}

double InterpolatingFramedClock::GetAverageFrameTime()
{
	// TODO: ¸É§¹
	return 0.0;
}

double InterpolatingFramedClock::GetFramesPerSecond()
{
	// TODO: ¸É§¹
	return 0.0;
}

FrameTimeInfo * InterpolatingFramedClock::GetFrameTimeInfo()
{
	FrameTimeInfo* timeInfo = new FrameTimeInfo();
	timeInfo->Current = GetCurrentTime();
	timeInfo->Elapsed = GetElapsedFrameTime();
	return timeInfo;
}
