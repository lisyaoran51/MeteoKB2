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
	LOG(LogLevel::Debug) << [=]() {
		if (clock->GetIsStarted())
			if (int(clock->GetCurrentTime()) > currentSecond) {
				currentSecond = int(clock->GetCurrentTime());
				LOG(LogLevel::Debug) << "GameThread::processFrame() : [" << threadName << "] frame rate = " << statisticFrameRate;
				statisticFrameRate = 0;
			}
		return 0;
	}();
	

	LOG(LogLevel::Depricated) << "GameThread::processFrame() [" << threadName << "]";


	if(clock->GetIsRunning())
		LOG(LogLevel::Depricated) << "GameThread::processFrame() : current time = [" << fixed << setprecision(5) << clock->GetCurrentTime() << "] by [" << threadName << "].";

	//if(threadName == "UpdateThread")
		LOG(LogLevel::Depricated) << "GameThread::processFrame() : [" << threadName << "] processing.";

	while (!ThreadMaster::GetInstance().CheckThreadProcessable(threadName));
	ThreadMaster::GetInstance().SetThreadProcessStatus(threadName, true);
	
	onNewFrame();
	
	ThreadMaster::GetInstance().SetThreadProcessStatus(threadName, false);


	clock->ProcessFrame();

	statisticFrameRate++;
	return 0;
}
