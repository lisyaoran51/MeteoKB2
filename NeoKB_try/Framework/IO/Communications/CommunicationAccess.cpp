#include "CommunicationAccess.h"


using namespace Framework::IO::Communications;



CommunicationAccess::CommunicationAccess(Host * gHost) : RegisterType("CommunicationAccess"), CommunicationCollectionManager(gHost)
{
	setHost(gHost);
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
