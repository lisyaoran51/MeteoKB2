#include "KeyboardDevice.h"


using namespace Framework::Devices;


bool KeyboardDevice::match(Peripheral * peripheral)
{
	if (dynamic_cast<Keyboard*>(peripheral))
		return true;
	return false;
}

int KeyboardDevice::handlePeripheralRegister(Peripheral * peripheral)
{
	matchedPeripheral = peripheral;
	matchedKeyboard = dynamic_cast<Keyboard*>(peripheral);
	peripheral->SetDevice(this);
	return 0;
}
