#include "DisplayDevice.h"

using namespace Framework::Devices;



bool DisplayDevice::match(Peripheral * peripheral)
{
	if (dynamic_cast<Display*>(peripheral))
		return true;

	return false;
}

int DisplayDevice::handlePeripheralRegister(Peripheral * peripheral)
{
	matchedPeripheral = peripheral;
	matchedDisplay = dynamic_cast<Display*>(peripheral);
	peripheral->SetDevice(this);
	return 0;
}
