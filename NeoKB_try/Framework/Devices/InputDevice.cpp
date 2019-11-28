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
	LOG(LogLevel::Debug) << "InputDevice::passToPeripheral() : pass to panel [" << inputStates.size() << "] states.";
	for (int i = 0; i < inputStates.size(); i++) {
		peripheral->PushInputState(inputStates[i]);
	}
	return 0;
}

