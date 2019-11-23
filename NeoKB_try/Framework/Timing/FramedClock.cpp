#include "FramedClock.h"

#include "StopwatchClock.h"
#include <stdexcept>
#include "../../Util/Log.h"
#include <iomanip>
#include <typeinfo>



using namespace Framework::Timing;
using namespace std;
using namespace Util;


FramedClock::FramedClock(Clock * s)
{
	LOG(LogLevel::Debug) << "FramedClock::FramedClock() : address = [" << this << "].";
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
	if (!isStarted)
		LOG(LogLevel::Debug) << "FramedClock::ProcessFrame() : not start yet -> address = [" << this << "], type = [" << typeid(this).name() << "].";
	else
		LOG(LogLevel::Debug) << "FramedClock::ProcessFrame() : current time [" << fixed << setprecision(5) << currentTime << "].";

	if (isProcessSourceClockFrames) {
		FrameBasedClock* sourceAsFrameBased = dynamic_cast<FrameBasedClock*>(source);
		if (sourceAsFrameBased)
			sourceAsFrameBased->ProcessFrame();
	}
	// TODO: 計算averageFrameTime和framesPerSecond

	if (!isStarted) {
		start();
		LOG(LogLevel::Finest) << "FramedClock::ProcessFrame() : current time [" << fixed << setprecision(5) << currentTime << "] after start";
	}
	

	lastFrameTime = currentTime;
	currentTime = source->GetCurrentTime();

	LOG(LogLevel::Finest) << "FramedClock::ProcessFrame() : current time [" << fixed << setprecision(5) << currentTime << "] after process";
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
	if(!isStarted)
		throw logic_error("int FramedClock::GetCurrentTime() : error. The clock is not Started.");
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

double FramedClock::GetLastFrameTime()
{
	return lastFrameTime;
}

double FramedClock::GetElapsedFrameTime()
{
	return currentTime - lastFrameTime;
}

double FramedClock::GetAverageFrameTime()
{
	// TODO: 先不寫
	return averageFrameTime;
}

double FramedClock::GetFramesPerSecond()
{
	// TODO: 先不寫
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

int FramedClock::setInnerCurrentTime(double cTime)
{
	currentTime = cTime;
	return 0;
}

int FramedClock::start()
{
	

	currentTime = source->GetCurrentTime();
	LOG(LogLevel::Finest) << "FramedClock::start() : start the clock. current time = " << currentTime;
	isStarted = true;
	return 0;
}
