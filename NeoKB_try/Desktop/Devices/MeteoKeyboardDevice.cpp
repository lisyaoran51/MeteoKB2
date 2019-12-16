#include "MeteoKeyboardDevice.h"

using namespace Desktop::Devices;


MeteoKeyboardDevice::MeteoKeyboardDevice(MeteoPanelBoardV1 * panelBoard)
{
	meteoPanelBoard = panelBoard;
}

int MeteoKeyboardDevice::readFromDevice()
{
#if 0
	InputState* newState = meteoPanelBoard->GetKeyboardState();
	if(newState != nullptr)
		inputStates.push_back(newState);

#else
	_debugCount++;
#endif

	if (_debugCount % 100 == 89) {

		InputState* newState = new InputState();
		newState->SetKeyboardState(new KeyboardState());

		//newState->GetKeyboardState()->AddPress(make_pair(InputKey::VK24_L_G1, 100));
		//newState->GetKeyboardState()->AddPress(make_pair(InputKey::VK24_L_E1, 100));
		newState->GetKeyboardState()->AddPress(make_pair(InputKey::VK24_L_C1, 100));
		inputStates.push_back(newState);

		LOG(LogLevel::Depricated) << "MeteoKeyboardDevice::readFromDevice() : Create fake kb input.";
	}

	if (_debugCount % 100 == 99) {

		InputState* newState = new InputState();
		newState->SetKeyboardState(new KeyboardState());

		//newState->GetKeyboardState()->AddUp(InputKey::VK24_L_G1);
		//newState->GetKeyboardState()->AddUp(InputKey::VK24_L_E1);
		//newState->GetKeyboardState()->AddUp(InputKey::VK24_L_C1);
		inputStates.push_back(newState);

		LOG(LogLevel::Depricated) << "MeteoKeyboardDevice::readFromDevice() : Create fake kb input.";
	}


	return 0;
}