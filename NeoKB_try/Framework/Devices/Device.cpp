#include "Device.h"

int Device::Register(MainInterface * mainInterface)
{
	vector<Peripheral*>* peripherals = mainInterface->GetPeripherals();

	for (int i = 0; i < peripherals->size(); i++) {
		if (match(peripherals->at(i))) {
			handlePeripheralRegister(peripherals->at(i));
			return 0;
		}
	}


	return -1;
}
