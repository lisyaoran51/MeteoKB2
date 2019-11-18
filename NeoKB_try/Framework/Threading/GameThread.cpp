#include "GameThread.h"

#include <iomanip>



using namespace Framework::Threading;





GameThread::GameThread(function<int()> oNewFrame, string tName)
{
	LOG(LogLevel::Info) << "GameThread::GameThread() : " << tName << " create clock and thread.";

	onNewFrame = oNewFrame;
	threadName = tName;

	clock = new ThrottledFramedClock();

	

}

int GameThread::Start()
{
	LOG(LogLevel::Info) << "GameThread::Start() : start thread " << threadName << ".";
	clock->ProcessFrame();
	LOG(LogLevel::Info) << "GameThread::Start() : start thread 2" << threadName << ".";
	runThread = new thread(&GameThread::runWork, this);
	runThread->detach();
	return 0;
}

ThrottledFramedClock * GameThread::GetClock()
{
	return clock;
}

int GameThread::SetMaxUpdateHz(double mUpdateHz)
{
	activeHz = mUpdateHz;
	if(isActive)
		clock->SetMaxUpdateHz(mUpdateHz);
	return 0;
}

double GameThread::GetMaxUpdateHz()
{
	return clock->GetMaxUpdateHz();
}

int GameThread::SetIsActive(bool value)
{
	if (value) {
		SetMaxUpdateHz(activeHz);
	}
	else {
		SetMaxUpdateHz(inactiveHz);
	}
	isActive = value;
	return 0;
}

int GameThread::runWork()
{
	while (!exitRequested)
		processFrame();
	return 0;
}

int GameThread::processFrame()
{


	LOG(LogLevel::Finest) << "GameThread::processFrame() [" << threadName << "]";


	if(clock->GetIsRunning())
		LOG(LogLevel::Finest) << "GameThread::processFrame() : current time = " << fixed << setprecision(5) << clock->GetCurrentTime() << ", last frame time = "
			<< clock->GetLastFrameTime() << ", process time = " << clock->GetElapsedFrameTime();

	onNewFrame();
	clock->ProcessFrame();

	return 0;
}
