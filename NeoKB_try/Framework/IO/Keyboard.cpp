#include "Keyboard.h"

#include "../Devices/KeyboardDevice.h"


using namespace Framework::IO;
using namespace Framework::Devices;



int Keyboard::SetDevice(Device * device)
{
	Peripheral::SetDevice(device);
	matchedKeyboardDevice = dynamic_cast<KeyboardDevice*>(device);
	return 0;
}

int Keyboard::TriggerOnInput()
{
	if (inputStates.size() > 0)
		LOG(LogLevel::Depricated) << "Keyboard::TriggerOnInput() : trigger on kb event.";

	for (int i = 0; i < inputStates.size(); i++) {
		OnKeyEvent.Trigger(inputStates[i]);
	}
	inputStates.clear();
	return 0;
}

ActionList<int, InputState*>* Keyboard::_DebugGetActionList() {
	return &OnKeyEvent;
}
