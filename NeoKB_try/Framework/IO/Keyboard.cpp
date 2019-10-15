#include "Keyboard.h"

using namespace Framework::IO;


template<class _Type>
int Keyboard::AddOnKeyEvent(_Type * callableObject, function<int(InputState*)> callback, string name = "HandleKeyEvent")
{
	OnKeyEvent.Add(callableObject, callback, name);

	return 0;
}

int Keyboard::SetDevice(Device * device)
{
	Peripheral::SetDevice(device);
	matchedKeyboardDevice = dynamic_cast<KeyboardDevice*>(device);
	return 0;
}

int Keyboard::TriggerOnInput()
{
	for (int i = 0; i < inputStates.size(); i++) {
		OnKeyEvent.Trigger(inputStates[i]);
	}
	inputStates.clear();
	return 0;
}
