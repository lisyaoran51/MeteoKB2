#include "StopwatchClock.h"


using namespace std::chrono;
using namespace Framework::Timing;


StopwatchClock::StopwatchClock()
{
	isRunning = false;
	isStarted = false;
	systemCurrentTime = system_clock::from_time_t(0);
	systemStartTime = system_clock::from_time_t(0);
	rateChangeAccumulatedTime = 0;
}

double StopwatchClock::GetCurrentTime()
{
	return (double(GetElapsedMilliseconds()) / 1000.0) * rate + rateChangeAccumulatedTime + seekOffset;
}

int StopwatchClock::SetRate(double r)
{

	rateChangeAccumulatedTime += GetElapsedSeconds() * rate;
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
	
	if (!isStarted || !isRunning) {
		if (!isStarted && isRunning)
			return -1; // TODO: throw error因為是異常狀況
		systemCurrentTime = system_clock::now();
		systemStartTime = system_clock::now();
		isRunning = true;
		isStarted = true;
	}
	
	return 0;
}

int StopwatchClock::Stop()
{
	if (!isStarted || !isRunning)
		return -1;
	rateChangeAccumulatedTime += GetElapsedSeconds() * rate;
	isRunning = false;
	return 0;
}

bool StopwatchClock::Seek(double position)
{
	seekOffset = position - GetCurrentTime();
	return true;
}

int StopwatchClock::ResetSpeedAdjustments()
{
	return SetRate(1);
}

long long StopwatchClock::GetElapsedMilliseconds()
{
	if(isRunning)
		systemCurrentTime = system_clock::now();
	return duration_cast<milliseconds>(systemCurrentTime - systemStartTime).count();
}

double StopwatchClock::GetElapsedSeconds()
{
	return double(GetElapsedMilliseconds()) / 1000.0;
}

