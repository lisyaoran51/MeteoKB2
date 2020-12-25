#include "BleAccess.h"


using namespace Games::IO::Communications;


BleAccess::BleAccess(GameHost * gHost): TCommunicationComponent(gHost), RegisterType("BleAccess")
{
	bluetoothPhone = host->GetMainInterface()->GetBluetoothPhone();
}

int BleAccess::handleRequest(CommunicationRequest * communicationRequest)
{
	return 0;
}
