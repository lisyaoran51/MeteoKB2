#include "BluetoothDevice.h"


using namespace Framework::Devices;




bool BluetoothDevice::match(Peripheral * peripheral)
{
	if (dynamic_cast<BluetoothPhone*>(peripheral))
		return true;
	return false;
}

int BluetoothDevice::handlePeripheralRegister(Peripheral * peripheral)
{
	matchedPeripheral = peripheral;
	matchedBluetoothPhone = dynamic_cast<BluetoothPhone*>(peripheral);
	peripheral->SetDevice(this);
	return 0;
}
