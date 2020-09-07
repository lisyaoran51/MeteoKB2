#include "MeteoBluetoothDevice.h"

using namespace Desktop::Devices;


MeteoBluetoothDevice::MeteoBluetoothDevice(MeteoBluetoothPhoneV1 * mBluetoothPhone)
{
	meteoBluetoothPhone = mBluetoothPhone;
}

int MeteoBluetoothDevice::AddOnStartWritingSmFile(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return meteoBluetoothPhone->AddOnStartWritingSmFile(callableObject, callback, name);
}

int MeteoBluetoothDevice::AddOnWriteSmFileSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return meteoBluetoothPhone->AddOnWriteSmFileSuccess(callableObject, callback, name);
}

int MeteoBluetoothDevice::AddOnStartWritingSoundFilePackage(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return meteoBluetoothPhone->AddOnStartWritingSoundFilePackage(callableObject, callback, name);
}

int MeteoBluetoothDevice::AddOnWriteSoundFilePackageSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return meteoBluetoothPhone->AddOnWriteSoundFilePackageSuccess(callableObject, callback, name);
}

int MeteoBluetoothDevice::AddOnStartWritingProgram(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return meteoBluetoothPhone->AddOnStartWritingProgram(callableObject, callback, name);
}

int MeteoBluetoothDevice::AddOnWriteProgramSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return meteoBluetoothPhone->AddOnWriteProgramSuccess(callableObject, callback, name);
}

int MeteoBluetoothDevice::readFromDevice()
{
	return 0;
}

int MeteoBluetoothDevice::passToDevice()
{
	for (int i = 0; i < outputMessages.size(); i++) {
		LOG(LogLevel::Depricated) << "MeteoPanelDevice::passToDevice() : pass message to board.";
		meteoBluetoothPhone->PushOutputMessage(outputMessages[i]);
		delete outputMessages[i];
	}
	outputMessages.clear();

	return 0;
}
