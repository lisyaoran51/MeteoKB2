#include "BluetoothInputHandler.h"

#include "../../IO/BluetoothPhone.h"


using namespace Framework::Input::Handler;
using namespace Framework::IO;



int BluetoothInputHandler::Initialize(GameHost * host)
{
	LOG(LogLevel::Info) << "BluetoothInputHandler::Initialize() : adding handler.";

	host->GetMainInterface()->GetBluetoothPhone()->
		AddOnCommand<BluetoothInputHandler>(this, bind((int(BluetoothInputHandler::*)(InputState*))&BluetoothInputHandler::HandleState, this, placeholders::_1), "BluetoothInputHandler::HandleState");



	return 0;
}

int BluetoothInputHandler::HandleState(InputState * inputState)
{
	LOG(LogLevel::Debug) << "BluetoothInputHandler::HandleState() : cloning new state and pend.";

	auto bluetoothState = inputState->GetBluetoothState()->Clone();

	InputState* pendingState = new InputState();
	pendingState->SetBluetoothState(bluetoothState);

	pendingStates.push_back(pendingState);

	return 0;
}
