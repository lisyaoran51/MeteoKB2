#include "ThrottledFramedClock.h"

#include <stdexcept>
#include <unistd.h>
#include "../../Util/Log.h"
#include <iomanip>
#include <thread>
#include <chrono>



using namespace Framework::Timing;
using namespace std;
using namespace Util;


int ThrottledFramedClock::throttle()
{

	int timeToSleepFlooredInMilli;

	double frameTime = 1.0 / maxUpdateHz;

	LOG(LogLevel::Depricated) << "ThrottledFramedClock::throttle(): elapsed frame time is [" << fixed << setprecision(5) << GetElapsedFrameTime() << "].";

	if (GetElapsedFrameTime() < frameTime) {
		double timeToSleepInMilli = (frameTime - GetElapsedFrameTime()) * 1000.0;

		timeToSleepFlooredInMilli = int(timeToSleepInMilli);

		accumulatedSleepError += timeToSleepInMilli - timeToSleepFlooredInMilli;

		
		int accumulatedSleepErrorFloored = (int)accumulatedSleepError;

		if (accumulatedSleepErrorFloored > 0) {
			accumulatedSleepError -= accumulatedSleepErrorFloored;
			timeToSleepInMilli += accumulatedSleepErrorFloored;
		}

		LOG(LogLevel::Depricated) << "ThrottledFramedClock::throttle() : time to sleep [" << fixed << setprecision(5) << timeToSleepInMilli / 1000.0 << "].";

		if (timeToSleepInMilli > 0) {
			//usleep(timeToSleepInMilli * 1000);
			this_thread::sleep_for(chrono::milliseconds((long)timeToSleepInMilli));
		}
		// 把睡眠以後的時間差記下來，把誤差累加進去
		double afterSleepTimeInMilli = source->GetCurrentTime() * 1000.0;
		accumulatedSleepError = timeToSleepFlooredInMilli - (afterSleepTimeInMilli - GetCurrentTime());

	}

	LOG(LogLevel::Debug) << "ThrottledFramedClock::throttle() : current time [" << fixed << setprecision(5) << source->GetCurrentTime() << "] after throttle";

	return 0;
}

ThrottledFramedClock::ThrottledFramedClock(Clock * s): FramedClock(s)
{
	LOG(LogLevel::Debug) << "ThrottledFramedClock::ThrottledFramedClock() : address = [" << this << "].";

	if(s)
	if(dynamic_cast<FrameBasedClock*>(s))
		throw runtime_error("ThrottledFramedClock::ThrottledFramedClock() : error. the source clock cannot be frame based.");
}

ThrottledFramedClock::ThrottledFramedClock(double mUpdateHz, Clock * s): FramedClock(s)
{
	SetMaxUpdateHz(mUpdateHz);
}

int ThrottledFramedClock::ProcessFrame()
{
	LOG(LogLevel::Depricated) << "ThrottledFramedClock::ProcessFrame(): this address is [" << this << "].";

	FramedClock::ProcessFrame();

	throttle();

	return 0;
}

int ThrottledFramedClock::SetMaxUpdateHz(double hz)
{
	if (hz > 1000)
		hz = 1000;
	maxUpdateHz = hz;
	return 0;
}

double ThrottledFramedClock::GetMaxUpdateHz()
{
	return maxUpdateHz;
}

double ThrottledFramedClock::getThrottledElapsedFrameTime()
{
	setInnerCurrentTime(GetSource()->GetCurrentTime());
	return GetCurrentTime() - lastFrameTime;
}
