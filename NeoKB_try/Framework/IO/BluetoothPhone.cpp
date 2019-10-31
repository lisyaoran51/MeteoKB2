#include "BluetoothPhone.h"

#include "../Devices/BluetoothDevice.h"



using namespace Framework::IO;
using namespace Framework::Devices;




int BluetoothPhone::SetDevice(Device * device)
{
	Peripheral::SetDevice(device);
	matchedBluetoothDevice = dynamic_cast<BluetoothDevice*>(device);
	return 0;
}

int BluetoothPhone::TriggerOnInput()
{
	for(int i = 0; i < inputStates.size(); i++)
		OnCommand.Trigger(inputStates[i]);

	inputStates.clear();
	return 0;
}
