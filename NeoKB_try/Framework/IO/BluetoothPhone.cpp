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
	for (int i = 0; i < inputStates.size(); i++) {
		OnCommand.Trigger(inputStates[i]);
		LOG(LogLevel::Depricated) << "BluetoothPhone::TriggerOnInput() : bt command .";
	}

	inputStates.clear();
	return 0;
}


int BluetoothPhone::AddOnStartWritingSmFile(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return matchedBluetoothDevice->AddOnStartWritingSmFile(callableObject, callback, name);
}

int BluetoothPhone::AddOnWriteSmFileSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	LOG(LogLevel::Depricated) << "BluetoothPhone::AddOnWriteSmFileSuccess() : api access setup. ";
	return matchedBluetoothDevice->AddOnWriteSmFileSuccess(callableObject, callback, name);
}

int BluetoothPhone::AddOnStartWritingSoundFilePackage(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return matchedBluetoothDevice->AddOnStartWritingSoundFilePackage(callableObject, callback, name);
}

int BluetoothPhone::AddOnWriteSoundFilePackageSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return matchedBluetoothDevice->AddOnWriteSoundFilePackageSuccess(callableObject, callback, name);
}

int BluetoothPhone::AddOnStartWritingProgram(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return matchedBluetoothDevice->AddOnStartWritingProgram(callableObject, callback, name);
}

int BluetoothPhone::AddOnWriteProgramSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return matchedBluetoothDevice->AddOnWriteProgramSuccess(callableObject, callback, name);
}

