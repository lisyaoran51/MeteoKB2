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

#if 0
	_debugCount++;
#endif

	if (_debugCount % 1000 == 0) {

		InputState* newState = new InputState();
		newState->SetKeyboardState(new KeyboardState());

		newState->GetKeyboardState()->AddPress(make_pair(InputKey::VK24_L_E1, 100));
		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoKeyboardDevice::readFromDevice() : Create fake kb input.";
	}


	return 0;
}