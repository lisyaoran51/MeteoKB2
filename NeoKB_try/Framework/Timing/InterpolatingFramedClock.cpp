#include "InterpolatingFramedClock.h"

#include "StopwatchClock.h"
#include <stdexcept>
#include <cmath>


using namespace Framework::Timing;
using namespace std;



InterpolatingFramedClock::InterpolatingFramedClock(Clock * s)
{
	interpolateClock = new FramedClock(new StopwatchClock());
	ChangeSource(s);

	allowableErrorInSeconds = 1.0 / 60 * 2;
}

int InterpolatingFramedClock::ProcessFrame()
{
	if (framedSource == nullptr)
		return 0;

	/* 更新速度 */
	interpolateClock->SetRate(rate = source->GetRate());

	/* 跑一禎 */
	interpolateClock->ProcessFrame();
	framedSource->ProcessFrame();
	
	sourceIsRunning = framedSource->GetIsRunning();

	lastInterpolatedTime = GetCurrentTime();

	if (!framedSource->GetIsRunning())
		return 0;

	currentInterpolatedTime += interpolateClock->GetElapsedFrameTime();

	if (fabs(framedSource->GetCurrentTime() - currentInterpolatedTime) > allowableErrorInSeconds)
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
		s->SetRate(rate);
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
	// TODO: log也打一下error
	// throw logic_error("int InterpolatingFramedClock::SetRate() : error. interpolated clock is not able to set rate.");
	interpolateClock->SetRate(r);
	rate = r;

	if (source == nullptr)
		return -1;

	framedSource->SetRate(r);
	return 0;
}

double InterpolatingFramedClock::GetRate()
{
	return rate;
}

int InterpolatingFramedClock::SetIsRunning(bool value)
{
	framedSource->SetIsRunning(value);
	interpolateClock->SetIsRunning(value);
	sourceIsRunning = value;
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
	// TODO: 補完
	return 0.0;
}

double InterpolatingFramedClock::GetFramesPerSecond()
{
	// TODO: 補完
	return 0.0;
}

FrameTimeInfo * InterpolatingFramedClock::GetFrameTimeInfo()
{
	FrameTimeInfo* timeInfo = new FrameTimeInfo();
	timeInfo->Current = GetCurrentTime();
	timeInfo->Elapsed = GetElapsedFrameTime();
	return timeInfo;
}
