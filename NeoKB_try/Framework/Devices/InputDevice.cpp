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
	if(inputStates.size() > 0)
		LOG(LogLevel::Depricated) << "InputDevice::passToPeripheral() : pass to device [" << inputStates.size() << "] states.";

	for (int i = 0; i < inputStates.size(); i++) {
		peripheral->PushInputState(inputStates[i]);
	}
	return 0;
}

