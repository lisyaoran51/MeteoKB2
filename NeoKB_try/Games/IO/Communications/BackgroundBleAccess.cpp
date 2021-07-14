#include "BackgroundBleAccess.h"


using namespace Games::IO::Communications;
using namespace Framework::Threading;


BackgroundBleAccess::BackgroundBleAccess(Host * gHost): TBleAccess<BleRequest>(gHost), RegisterType("BackgroundBleAccess")
{
	ThreadMaster::GetInstance().AddSimpleThread(this);
	
}

/*
int BackgroundBleAccess::Queue(CommunicationRequest * communicationRequest)
{
	return TCommunicationComponent<BackgroundGetBinaryBleRequest>::Queue(communicationRequest);
}

int BackgroundBleAccess::Flush()
{
	return TCommunicationComponent<BackgroundGetBinaryBleRequest>::Flush();
}
*/