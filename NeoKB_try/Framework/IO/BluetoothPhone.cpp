#include "BluetoothPhone.h"

int BluetoothPhone::SetDevice(Device * device)
{
	Peripheral::SetDevice(device);
	matchedBluetoothDevice = dynamic_cast<BluetoothDevice*>(device);
	return 0;
}

int BluetoothPhone::TriggerOnInput()
{

	OnCommand.Trigger(inputStates);

	inputStates.clear();
	return 0;
}

template<class _Type>
int BluetoothPhone::AddOnCommand(_Type * callableObject, function<int(InputState*)> callback, string name)
{
	OnCommand.Add(callableObject, callback, name);
	return 0;
}