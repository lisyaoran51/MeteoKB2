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

/*
template<class _Type>
int BluetoothPhone::AddOnCommand(_Type* callableObject, function<int(InputState*)> callback, string name) {

	LOG(LogLevel::Fine) << "BluetoothPhone::AddOnCommand() : register handler into list.";

	OnCommand.Add(callableObject, callback, name);
	return 0;
}
*/

template<class _Type>
int BluetoothPhone::AddOnStartWritingSmFile(_Type * callableObject, function<int(string)> callback, string name)
{
	return matchedBluetoothDevice->AddOnStartWritingSmFile(callableObject, callback, name);
}
template int BluetoothPhone::AddOnStartWritingSmFile<MtoObject>(MtoObject * callableObject, function<int(string)> callback, string name);

template<class _Type>
int BluetoothPhone::AddOnWriteSmFileSuccess(_Type * callableObject, function<int(string)> callback, string name)
{
	return matchedBluetoothDevice->AddOnWriteSmFileSuccess(callableObject, callback, name);
}
template int BluetoothPhone::AddOnWriteSmFileSuccess<MtoObject>(MtoObject * callableObject, function<int(string)> callback, string name);

template<class _Type>
int BluetoothPhone::AddOnStartWritingSoundFilePackage(_Type * callableObject, function<int(string)> callback, string name)
{
	return matchedBluetoothDevice->AddOnStartWritingSoundFilePackage(callableObject, callback, name);
}

template<class _Type>
int BluetoothPhone::AddOnWriteSoundFilePackageSuccess(_Type * callableObject, function<int(string)> callback, string name)
{
	return matchedBluetoothDevice->AddOnWriteSoundFilePackageSuccess(callableObject, callback, name);
}

template<class _Type>
int BluetoothPhone::AddOnStartWritingProgram(_Type * callableObject, function<int(string)> callback, string name)
{
	return matchedBluetoothDevice->AddOnStartWritingProgram(callableObject, callback, name);
}

template<class _Type>
int BluetoothPhone::AddOnWriteProgramSuccess(_Type * callableObject, function<int(string)> callback, string name)
{
	return matchedBluetoothDevice->AddOnWriteProgramSuccess(callableObject, callback, name);
}

