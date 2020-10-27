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
	LOG(LogLevel::Depricated) << "MeteoBluetoothDevice::AddOnWriteSmFileSuccess() : api access setup. ";
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
#if 0

	InputState* newState = meteoBluetoothPhone->GetBluetoothState();
	if (newState != nullptr) {
		LOG(LogLevel::Debug) << "MeteoBluetoothDevice::readFromDevice() : got new bt state. ";
		inputStates.push_back(newState);
	}

#else
	_debugCount++;
#endif

	if (_debugCount == 3000) {

		InputState* newState = new InputState();
		newState->SetBluetoothState(new BluetoothState());
		MeteoBluetoothCommand* btCommand = new MeteoBluetoothCommand(MeteoCommand::RequestLoadGame);
		btCommand->GetContext()["FileName"] = "LittleStar.sm";

		LOG(LogLevel::Debug) << "MeteoBluetoothDevice::readFromDevice() : Create fake bt input [" << btCommand->GetContext().dump() << "].";

		newState->GetBluetoothState()->AddCommand(btCommand);

		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoBluetoothDevice::readFromDevice() : Create fake bt input.";
	}

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
