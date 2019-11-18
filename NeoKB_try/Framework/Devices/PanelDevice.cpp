#include "PanelDevice.h"

using namespace Framework::Devices;




bool PanelDevice::match(Peripheral * peripheral)
{
	if (dynamic_cast<Panel*>(peripheral))
		return true;
	return false;
}

int PanelDevice::handlePeripheralRegister(Peripheral * peripheral)
{
	matchedPeripheral = peripheral;
	matchedPanel = dynamic_cast<Panel*>(peripheral);
	peripheral->SetDevice(this);
	return 0;
}
