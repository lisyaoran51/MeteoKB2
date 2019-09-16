#include "InputDevice.h"


using namespace Framework::Devices;





int InputDevice::ScanInput()
{
	readFromDevice();
	passToPeripheral(matchedPeripheral);
	matchedPeripheral->TriggerOnInput();
	return 0;
}

