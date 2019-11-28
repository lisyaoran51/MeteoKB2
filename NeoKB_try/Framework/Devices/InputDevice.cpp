#include "InputDevice.h"


using namespace Framework::Devices;





int InputDevice::ScanInput()
{
	readFromDevice();
	passToPeripheral(matchedPeripheral);
	matchedPeripheral->TriggerOnInput();

	for (int i = 0; i < inputStates.size(); i++) {
		delete inputStates[i];
	}
	inputStates.clear();

	return 0;
}

int InputDevice::passToPeripheral(Peripheral * peripheral)
{
	for (int i = 0; i < inputStates.size(); i++) {
		peripheral->PushInputState(inputStates[i]);
	}
	return 0;
}

