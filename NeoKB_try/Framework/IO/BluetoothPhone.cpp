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
		bool isRawMessage = false;
		for (int j = 0; j < inputStates[i]->GetBluetoothState()->GetMessages()->size(); j++) {
			if (inputStates[i]->GetBluetoothState()->GetMessages()->at(j)->GetIsRawMessage())
				isRawMessage = true;
		}

		if(!isRawMessage)
			OnMessage.Trigger(inputStates[i]);

		OnRawMessage.Trigger(inputStates[i]);
		LOG(LogLevel::Depricated) << "BluetoothPhone::TriggerOnInput() : bt command .";
	}

	inputStates.clear();
	return 0;
}

bool BluetoothPhone::CheckFileSegmentMessageOutputClear()
{
	return matchedBluetoothDevice->CheckFileSegmentMessageOutputClear();
}

int BluetoothPhone::AddOnConnect(MtoObject * callableObject, function<int()> callback, string name)
{
	matchedBluetoothDevice->AddOnConnect(callableObject, callback, name);
	return 0;
}

int BluetoothPhone::AddOnDisonnect(MtoObject * callableObject, function<int()> callback, string name)
{
	matchedBluetoothDevice->AddOnDisconnect(callableObject, callback, name);
	return 0;
}

/*
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

*/