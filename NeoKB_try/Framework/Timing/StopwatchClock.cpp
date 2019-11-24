#include "StopwatchClock.h"

#include "../../Util/Log.h"



using namespace std::chrono;
using namespace Framework::Timing;
using namespace Util;


StopwatchClock::StopwatchClock()
{
	isRunning = false;
	isStarted = false;
	systemCurrentTime = system_clock::from_time_t(0);
	systemStartTime = system_clock::from_time_t(0);
	rateChangeAccumulatedTime = 0;
	seekOffset = 0;
}

double StopwatchClock::GetCurrentTime()
{
	if (!isStarted)
		Start();
	return getElapsedSeconds() * rate + rateChangeAccumulatedTime + seekOffset;
}

int StopwatchClock::SetRate(double r)
{

	rateChangeAccumulatedTime += getElapsedSeconds() * rate;
	systemStartTime = systemCurrentTime;
	rate = r;
	return 0;
}

double StopwatchClock::GetRate()
{
	return rate;
}

int StopwatchClock::SetIsRunning(bool value)
{
	if (isRunning && !value) {
		Stop();
	}
	else if (!isRunning && value) {
		Start();
	}
	isRunning = value;
	return 0;
}

bool StopwatchClock::GetIsRunning()
{
	return isRunning;
}

int StopwatchClock::Reset()
{
	rateChangeAccumulatedTime = 0;
	isRunning = false;
	isStarted = false;
	return 0;
}

int StopwatchClock::Restart()
{
	Reset();
	Start();
	return 0;
}

int StopwatchClock::Start()
{
	LOG(LogLevel::Finest) << "StopwatchClock::Start() : start the stopwatch.";
	if (!isStarted || !isRunning) {
		if (!isStarted && isRunning)
			return -1; // TODO: throw error因為是異常狀況
		systemStartTime = system_clock::now();
		systemCurrentTime = system_clock::now();
		isRunning = true;
		isStarted = true;
	}
	
	return 0;
}

int StopwatchClock::Stop()
{
	if (!isStarted || !isRunning)
		return -1;
	rateChangeAccumulatedTime += getElapsedSeconds() * rate;
	isRunning = false;
	return 0;
}

bool StopwatchClock::Seek(double position)
{
	LOG(LogLevel::Debug) << "StopwatchClock::Seek : go to position.";
	seekOffset = position - GetCurrentTime();
	return true;
}

int StopwatchClock::ResetSpeedAdjustments()
{
	return SetRate(1);
}

long long StopwatchClock::getElapsedMicroseconds()
{
	if(isRunning)
		systemCurrentTime = system_clock::now();
	return duration_cast<microseconds>(systemCurrentTime - systemStartTime).count();
}

double StopwatchClock::getElapsedSeconds()
{
	return double(getElapsedMicroseconds()) / 1000000.0;
}

