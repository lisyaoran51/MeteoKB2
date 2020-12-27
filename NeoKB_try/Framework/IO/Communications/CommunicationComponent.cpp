#include "CommunicationComponent.h"


using namespace Framework::IO::Communications;


CommunicationComponent::CommunicationComponent(GameHost * gHost): host(gHost)
{
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

	// TODO: ???�٤����D�n���g

	return 0;
}
