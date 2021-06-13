#include "ForegroundBleAccess.h"


using namespace Games::IO::Communications;


ForegroundBleAccess::ForegroundBleAccess(Host * gHost) : TBleAccess<BleRequest>(gHost), RegisterType("ForegroundBleAccess")
{
}

/*
int ForegroundBleAccess::Queue(CommunicationRequest * communicationRequest)
{
	return TCommunicationComponent<BleRequest>::Queue(communicationRequest);
}

int ForegroundBleAccess::Flush()
{
	return TCommunicationComponent<BleRequest>::Flush();
}
*/