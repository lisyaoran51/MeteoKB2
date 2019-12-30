#include "Device.h"

int Device::Register(MainInterface * mainInterface)
{
	if (matchedPeripheral)
		return -1;

	LOG(LogLevel::Info) << "Device::Register() : Register peripheral to interface.";

	vector<Peripheral*>* peripherals = mainInterface->GetPeripherals();

	for (int i = 0; i < peripherals->size(); i++) {
		if (match(peripherals->at(i))) {
			handlePeripheralRegister(peripherals->at(i));
			return 0;
		}
	}


	return -1;
}
