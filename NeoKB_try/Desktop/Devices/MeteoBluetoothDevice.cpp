#include "MeteoBluetoothDevice.h"

#include "../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"

using namespace Desktop::Devices;
using namespace Games::Output::Bluetooths;


MeteoBluetoothDevice::MeteoBluetoothDevice(MeteoBluetoothPhoneV2 * mBluetoothPhone)
{
	meteoBluetoothPhone = mBluetoothPhone;
	mBluetoothPhone->Initialize();
}

bool MeteoBluetoothDevice::CheckFileSegmentMessageOutputClear()
{
	// TODO: meteoBluetoothPhone->CheckFileSegmentMessageOutputClear()
	LOG(LogLevel::Error) << "MeteoBluetoothDevice::CheckFileSegmentMessageOutputClear() : not implemented. ";
	return false;
}

int MeteoBluetoothDevice::readFromDevice()
{
#if 1

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
		MeteoContextBluetoothMessage* btMessage = new MeteoContextBluetoothMessage(MeteoCommand::RequestLoadGame);
		json context;
		context["FileName"] = "LittleStar.sm";
		btMessage->SetContextInJson(context);

		//LOG(LogLevel::Debug) << "MeteoBluetoothDevice::readFromDevice() : Create fake bt input [" << btCommand->GetContext().dump() << "].";

		btMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
		newState->GetBluetoothState()->AddMessage(btMessage);

		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoBluetoothDevice::readFromDevice() : Create fake bt input.";
	}

	if (_debugCount == 30000) {

		InputState* newState = new InputState();
		newState->SetBluetoothState(new BluetoothState());
		MeteoContextBluetoothMessage* btMessage = new MeteoContextBluetoothMessage(MeteoCommand::AppSwitchPianoInstrument);
		json context;
		context["Instrument"] = "piano";
		btMessage->SetContextInJson(context);

		//LOG(LogLevel::Debug) << "MeteoBluetoothDevice::readFromDevice() : Create fake bt input [" << btCommand->GetContext().dump() << "].";

		btMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
		newState->GetBluetoothState()->AddMessage(btMessage);

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
