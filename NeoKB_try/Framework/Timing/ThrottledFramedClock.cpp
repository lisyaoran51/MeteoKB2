#include "ThrottledFramedClock.h"

#include <stdexcept>
#include <unistd.h>
#include "../../Util/Log.h"
#include <iomanip>



using namespace Framework::Timing;
using namespace std;
using namespace Util;


int ThrottledFramedClock::throttle()
{
	int timeToSleepFlooredInMilli;

	double frameTime = 1.0 / maxUpdateHz;

	if (GetElapsedFrameTime() < frameTime) {
		double timeToSleepInMilli = (frameTime - GetElapsedFrameTime()) * 1000.0;

		timeToSleepFlooredInMilli = int(timeToSleepInMilli);

		accumulatedSleepError += timeToSleepInMilli - timeToSleepFlooredInMilli;

		
		int accumulatedSleepErrorFloored = (int)accumulatedSleepError;

		if (accumulatedSleepErrorFloored > 0) {
			accumulatedSleepError -= accumulatedSleepErrorFloored;
			timeToSleepInMilli += accumulatedSleepErrorFloored;
		}

		LOG(LogLevel::Finest) << "ThrottledFramedClock::throttle() : time to sleep [" << fixed << setprecision(5) << timeToSleepInMilli / 1000.0 << "].";

		if (timeToSleepInMilli > 0) {
			usleep(timeToSleepInMilli * 1000);
		}
		// 把睡眠以後的時間差記下來，把誤差累加進去
		double afterSleepTimeInMilli = source->GetCurrentTime() * 1000.0;
		accumulatedSleepError = timeToSleepFlooredInMilli - (afterSleepTimeInMilli - GetCurrentTime());

	}

	return 0;
}

ThrottledFramedClock::ThrottledFramedClock(Clock * s): FramedClock(s)
{
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
