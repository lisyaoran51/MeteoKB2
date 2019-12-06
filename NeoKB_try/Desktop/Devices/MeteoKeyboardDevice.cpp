#include "MeteoKeyboardDevice.h"

using namespace Desktop::Devices;


int MeteoKeyboardDevice::readFromDevice()
{

	_debugCount++;

	if (_debugCount == 3000) {

		InputState* newState = new InputState();
		newState->SetKeyboardState(new KeyboardState());

		newState->GetKeyboardState()->AddPress(make_pair(InputKey::VK24_L_C2, 100));
		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoKeyboardDevice::readFromDevice() : Create fake kb input.";
	}


	return 0;
}