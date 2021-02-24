#include "CommunicationAccess.h"


using namespace Framework::IO::Communications;



CommunicationAccess::CommunicationAccess(GameHost * gHost) : RegisterType("CommunicationAccess"), CommunicationCollectionManager(gHost)
{
}

int CommunicationAccess::Queue(CommunicationRequest * communicationRequest)
{
	for (int i = 0; i < items.size(); i++) {
		items[i]->Queue(communicationRequest);
	}


	return communicationRequest->ChooseCommunicationComponentAndPerform();
}

int CommunicationAccess::handleRequest(CommunicationRequest * communicationRequest)
{
	// no-op
	return 0;
}
