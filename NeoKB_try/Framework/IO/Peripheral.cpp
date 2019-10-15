#include "Peripheral.h"

using namespace Framework::IO;

int Peripheral::SetDevice(Device * device)
{
	matchedDevice = device;
	return 0;
}

int Peripheral::PushInputState(InputState * inputState)
{
	inputStates.push_back(inputState);
	return 0;
}

