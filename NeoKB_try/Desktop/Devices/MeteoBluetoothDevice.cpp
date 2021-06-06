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

int MeteoBluetoothDevice::AddOnConnect(MtoObject * callableObject, function<int()> callback, string name)
{
	meteoBluetoothPhone->AddOnConnect(callableObject, callback, name);
	return 0;
}

int MeteoBluetoothDevice::AddOnDisconnect(MtoObject * callableObject, function<int()> callback, string name)
{
	meteoBluetoothPhone->AddOnDisconnect(callableObject, callback, name);
	return 0;
}

int MeteoBluetoothDevice::readFromDevice()
{
#if 1
	LOG(LogLevel::Depricated) << "MeteoBluetoothDevice::readFromDevice() : reading... ";
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
		MeteoContextBluetoothMessage* btMessage = new MeteoContextBluetoothMessage(MeteoCommand::AppSwitchKeyboardInstrument);
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
	unique_lock<mutex> uLock(outputMessageMutex);

	for (int i = 0; i < outputMessages.size(); i++) {
		// 判斷是否傳送成功，成功則>=0，失敗則惟<0
		int success = -1;
		try {
			LOG(LogLevel::Depricated) << "MeteoBluetoothDevice::passToDevice() : pass message to bt.";
			success = meteoBluetoothPhone->PushOutputMessage(dynamic_cast<BluetoothMessage*>(outputMessages[i]));
		}
		catch (exception &e) {
			LOG(LogLevel::Error) << "MeteoBluetoothDevice::passToDevice() : pushing output message exception : " << e.what();
			success = 0;
		}

		if (success >= 0) {
			delete outputMessages[i];
			outputMessages.erase(outputMessages.begin() + i);
			i--;
		}
		
	}
	//outputMessages.clear();

	return 0;
}
