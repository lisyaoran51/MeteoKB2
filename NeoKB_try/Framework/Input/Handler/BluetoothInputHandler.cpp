#include "BluetoothInputHandler.h"

using namespace Framework::Input::Handler;


int BluetoothInputHandler::Initialize(GameHost * host)
{
	host->GetMainInterface()->GetBluetoothPhone()->
		AddOnCommand<BluetoothInputHandler>(this, bind((int(BluetoothInputHandler::*)(InputState*))&BluetoothInputHandler::HandleState, this, placeholders::_1), "BluetoothInputHandler::HandleState");



	return 0;
}

int BluetoothInputHandler::HandleState(InputState * inputState)
{
	auto bluetoothState = inputState->GetBluetoothState()->Clone();

	InputState* pendingState = new InputState();
	pendingState->SetBluetoothState(bluetoothState);

	pendingStates.push_back(pendingState);

	return 0;
}
