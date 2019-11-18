#include "GameThread.h"

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
	onNewFrame();
	clock->ProcessFrame();

	return 0;
}
