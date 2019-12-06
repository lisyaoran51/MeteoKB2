#include "StopwatchClock.h"

#include "../../Util/Log.h"
#include <iomanip>



using namespace std::chrono;
using namespace Framework::Timing;
using namespace Util;
using namespace std;


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
	if (isStarted) {
		rateChangeAccumulatedTime += getElapsedSeconds() * rate;
		LOG(LogLevel::Depricated) << "StopwatchClock::SetRate() : accumulated time = [" << rateChangeAccumulatedTime << "], acccumulated in this setting = [" << getElapsedSeconds() * rate << "], rate = [" << rate << "], input rate = [" << r << "].";

		unique_lock<mutex> uLock(currentTimeMutex);
		systemStartTime = systemCurrentTime;
	}
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
	
	if (!isStarted || !isRunning) {
		LOG(LogLevel::Debug) << "StopwatchClock::Start() : start the stopwatch, started = [" << isStarted << "], running = [" << isRunning << "].";
		if (!isStarted && isRunning)
			return -1; // TODO: throw error因為是異常狀況
		systemStartTime = system_clock::now();
		systemCurrentTime = system_clock::now();
		LOG(LogLevel::Finest) << "StopwatchClock::Start() : current elapsed time = [" << duration_cast<microseconds>(systemCurrentTime - systemStartTime).count() << "].";
		isRunning = true;
		isStarted = true;
	}
	
	return 0;
}

int StopwatchClock::Stop()
{
	LOG(LogLevel::Debug) << "StopwatchClock::Stop() : stop this. by [" << this << "]";
	if (!isStarted || !isRunning)
		return -1;
	rateChangeAccumulatedTime += getElapsedSeconds() * rate;
	isRunning = false;
	return 0;
}

bool StopwatchClock::Seek(double position)
{
	LOG(LogLevel::Debug) << "StopwatchClock::Seek : go to position [" << position << "].";
	seekOffset += position - GetCurrentTime();
	LOG(LogLevel::Debug) << "StopwatchClock::Seek : after go to position [" << position << "], current time = " << fixed << setprecision(5) << GetCurrentTime() << ".";
	return true;
}

int StopwatchClock::ResetSpeedAdjustments()
{
	return SetRate(1);
}

long long StopwatchClock::getElapsedMicroseconds()
{
	if (isRunning) {
		unique_lock<mutex> uLock(currentTimeMutex);
		systemCurrentTime = system_clock::now();
	}

	LOG(LogLevel::Depricated) << "StopwatchClock::getElapsedMicroseconds() : current elapsed time = [" << duration_cast<microseconds>(systemCurrentTime - systemStartTime).count() << "].";

	unique_lock<mutex> uLock(currentTimeMutex);
	return duration_cast<microseconds>(systemCurrentTime - systemStartTime).count();
}

double StopwatchClock::getElapsedSeconds()
{
	long long eMicroSeconds = getElapsedMicroseconds();
	LOG(LogLevel::Depricated) << "StopwatchClock::getElapsedSeconds() : elapsed seconds = [" << double(eMicroSeconds / 1000000) + double(eMicroSeconds % 1000000) * 0.000001 << "]";
	return double(eMicroSeconds / 1000000) + double(eMicroSeconds % 1000000) * 0.000001;
}

