#include "MeteoKeyboardDevice.h"

using namespace Desktop::Devices;


MeteoKeyboardDevice::MeteoKeyboardDevice(MeteoPanelBoardV1 * panelBoard)
{
	meteoPanelBoard = panelBoard;
}

int MeteoKeyboardDevice::readFromDevice()
{

	InputState* newState = meteoPanelBoard->GetKeyboardState();
	if(newState != nullptr)
		inputStates.push_back(newState);


	_debugCount;


	if (_debugCount == 3000) {

		InputState* newState = new InputState();
		newState->SetKeyboardState(new KeyboardState());

		newState->GetKeyboardState()->AddPress(make_pair(InputKey::VK24_L_C2, 100));
		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoKeyboardDevice::readFromDevice() : Create fake kb input.";
	}


	return 0;
}