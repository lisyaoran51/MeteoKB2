#include "BleAccess.h"


using namespace Games::IO::Communications;


BleAccess::BleAccess(GameHost * gHost): TCommunicationComponent(gHost), RegisterType("BleAccess")
{
	bluetoothPhone = host->GetMainInterface()->GetBluetoothPhone();


}

Peripheral * BleAccess::GetPeripheral()
{
	return bluetoothPhone;
}

deque<BluetoothCommand*>& BleAccess::GetInputRawCommand()
{
	// TODO: 於此處插入傳回陳述式
	return inputRawCommand;
}

int BleAccess::run()
{
	switch (communicationState) {
	case CommunicationState::Offline:
		this_thread::sleep_for(std::chrono::milliseconds(500));
		return -1;
		break;

	case CommunicationState::Connecting:

		// if login
		communicationState = CommunicationState::Connected;
		// else sleep(500); return -1;

		break;

	}

	CommunicationRequest* request = nullptr;
	if (communicationRequests.size() > 0) {
		request = communicationRequests.back();

		// request處理成功
		if (handleRequest(request) >= 0) {
			communicationRequests.pop_back();
		}
		else
			return -1;

	}


	return 0;
}

int BleAccess::handleRequest(CommunicationRequest * communicationRequest)
{
	try {

		LOG(LogLevel::Fine) << "BleAccess::handleRequest() : run request [" << communicationRequest << "].";

		communicationRequest->Perform(this);

		failureCount = 0;

	}
	catch (BleRequestException& e) {

		BleResponseCode response = e.GetBleResponseCode();

		switch (response) {
		case BleResponseCode::RequestTimeout:
			failureCount++;

			if (failureCount < 3)
				return -1;

			communicationState == CommunicationState::Failed;
			Flush();
			return 0;

			break;
		}

		communicationRequest->Fail(e);

	}

	return 0;
}

int BleAccess::handleOnRawCommand(InputState * inputState)
{
	if (inputState->GetBluetoothState()->GetCommands()->size() > 0) {
		for (int i = 0; i < inputState->GetBluetoothState()->GetCommands()->size(); i++) {
			inputRawCommand.push_front(inputState->GetBluetoothState()->GetCommands()->at(i)->Clone());
		}
	}

	return 0;
}
