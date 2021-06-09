#include "CommunicationAccess.h"


using namespace Framework::IO::Communications;



CommunicationAccess::CommunicationAccess(Host * gHost) : RegisterType("CommunicationAccess"), CommunicationCollectionManager(gHost)
{
	// 這個本來應該要從game thread拿，但是因為這次我們不用game thread，所以就自己生一個
	sourceClock = new StopwatchClock();
	sourceClock->Start();
	InitializeFramedClockAndScheduler();
	setHost(gHost);
}

int CommunicationAccess::Update()
{
	return CommunicationCollectionManager<CommunicationComponent>::Update();
}

int CommunicationAccess::Queue(CommunicationRequest * communicationRequest)
{
	for (int i = 0; i < items.size(); i++) {
		items[i]->Queue(communicationRequest);
	}


	return communicationRequest->ChooseCommunicationComponentToPerform();
}

int CommunicationAccess::handleRequest(CommunicationRequest * communicationRequest)
{
	// no-op
	return 0;
}

int CommunicationAccess::HandleState(InputState * inputEvent)
{
	for (int i = 0; i < items.size(); i++) {
		items[i]->HandleState(inputEvent);
	}
	return 0;
}

int CommunicationAccess::setHost(Host * gHost)
{
	bleCommunicationInputHandler = new BleCommunicationInputHandler(this);
	bleCommunicationInputHandler->Initialize(gHost);

	return 0;
}
