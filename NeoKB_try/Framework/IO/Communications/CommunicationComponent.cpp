#include "CommunicationComponent.h"


using namespace Framework::IO::Communications;


CommunicationComponent::CommunicationComponent(Host * gHost): host(gHost), RegisterType("CommunicationComponent")
{
}

int CommunicationComponent::Start()
{
	runThread = new thread(&CommunicationComponent::run, this);
	runThread->detach();

	// 要不要加入thread master? 避免thread safe問題
	// ThreadMaster::GetInstance().AddNewThread(threadName);

	return 0;
}

CommunicationState CommunicationComponent::GetCommunicationState()
{
	return communicationState;
}

GameThread * CommunicationComponent::GetCommunicationThread()
{
	return nullptr;
}

int CommunicationComponent::SetSourceClock(StopwatchClock * sClock)
{
	sourceClock = sClock;
	return 0;
}

StopwatchClock * CommunicationComponent::GetSourceClock()
{
	return sourceClock;
}

int CommunicationComponent::InitializeFramedClockAndScheduler()
{
	framedClock = new FramedClock(sourceClock);
	scheduler = new Scheduler(framedClock);

	return 0;
}

FrameBasedClock * CommunicationComponent::GetFramedClock()
{
	return framedClock;
}

Scheduler * CommunicationComponent::GetScheduler()
{
	return scheduler;
}

int CommunicationComponent::Update()
{
	if (scheduler != nullptr) {
		scheduler->Update();
	}

	return 0;
}

int CommunicationComponent::run()
{

	// TODO: ???還不知道要怎麼寫

	return 0;
}
