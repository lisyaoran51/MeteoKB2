#include "GameThread.h"

#include <iomanip>
#include <chrono>         // std::chrono::seconds
#include <sys/syscall.h>
#include <unistd.h>
#include "ThreadMaster.h"


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
	LOG(LogLevel::Depricated) << "GameThread::Start() : clock = [" << clock << "].";
	runThread = new thread(&GameThread::runWork, this);
	runThread->detach();

	ThreadMaster::GetInstance().AddNewThread(threadName);

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

bool GameThread::GetIsActive()
{
	return isActive;
}

int GameThread::runWork()
{
	this_thread::sleep_for(chrono::milliseconds(1000));

	threadId = syscall(SYS_gettid);
	LOG(LogLevel::Info) << "GameThread::runWork() : thread " << threadName << "'s ID is [" << threadId << "].";

	while (!exitRequested) 
		processFrame();


	return 0;
}

int GameThread::processFrame()
{
	if (clock->GetIsStarted())
		if (int(clock->GetCurrentTime()) > currentSecond) {
			currentSecond = int(clock->GetCurrentTime());
			LOG(LogLevel::Fine) << "GameThread::processFrame() : [" << threadName << "] frame rate = " << statisticFrameRate;
			statisticFrameRate = 0;
		}
	

	LOG(LogLevel::Depricated) << "GameThread::processFrame() [" << threadName << "]";


	if(clock->GetIsRunning())
		LOG(LogLevel::Depricated) << "GameThread::processFrame() : current time = [" << fixed << setprecision(5) << clock->GetCurrentTime() << "] by [" << threadName << "].";

	if(threadName == "InputThread")
		LOG(LogLevel::Depricated) << "GameThread::processFrame() : [" << threadName << "] processing.";

	while (!ThreadMaster::GetInstance().CheckThreadProcessable(threadName));
	ThreadMaster::GetInstance().SetThreadProcessStatus(threadName, true);
	
	onNewFrame();
	
	ThreadMaster::GetInstance().SetThreadProcessStatus(threadName, false);

	if (threadName == "InputThread")
		LOG(LogLevel::Depricated) << "GameThread::processFrame() : [" << threadName << "] new frame over.";

	try {
		clock->ProcessFrame();
	}
	catch (exception& e) {
		LOG(LogLevel::Error) << "GameThread::processFrame() : clock error " << e.what();
		throw e;
	}

	if (threadName == "InputThread")
		LOG(LogLevel::Depricated) << "GameThread::processFrame() : [" << threadName << "] clock process over.";

	statisticFrameRate++;
	return 0;
}
