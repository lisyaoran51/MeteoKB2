#include "GameThread.h"

using namespace Framework::Threading;





GameThread::GameThread(function<int()> oNewFrame, string tName)
{
	onNewFrame = oNewFrame;
	threadName = tName;


	runThread = new thread(&GameThread::runWork, this);

}

int GameThread::Start()
{
	runThread->detach();
	return 0;
}

ThrottledFramedClock * GameThread::GetClock()
{
	return clock;
}

int GameThread::SetSleepTime(int sTime)
{
	sleepTime = sTime;
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

	return 0;
}
