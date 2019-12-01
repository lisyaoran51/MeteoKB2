#include "InputState.h"



using namespace  Framework::Input;

InputState::~InputState()
{
	if (bluetoothState != nullptr)
		delete bluetoothState;
	if (keyboardState != nullptr)
		delete keyboardState;
	if (panelState != nullptr)
		delete panelState;
}

InputState * InputState::Clone()
{
	InputState * cloned = new InputState();

	if(lastState != nullptr)
		cloned->SetLastState(lastState);

	if (bluetoothState != nullptr)
		cloned->SetBluetoothState(bluetoothState->Clone());
	if (keyboardState != nullptr)
		cloned->SetKeyboardState(keyboardState->Clone());
	if (panelState != nullptr)
		cloned->SetPanelState(panelState->Clone());


	return cloned;
}

int InputState::SetLastState(InputState * lState)
{
	lastState = lState;
	if(lState != nullptr)
		lState->SetIsLastState();

	return 0;
}

InputState * InputState::GetLastState()
{
	return lastState;
}

int InputState::SetKeyboardState(KeyboardState * kState)
{
	keyboardState = kState;
	return 0;
}

KeyboardState * InputState::GetKeyboardState()
{
	return keyboardState;
}

int InputState::SetPanelState(PanelState * pState)
{
	panelState = pState;
	return 0;
}

PanelState * InputState::GetPanelState()
{
	return panelState;
}

int InputState::SetBluetoothState(BluetoothState * bState)
{
	bluetoothState = bState;
	return 0;
}

BluetoothState * InputState::GetBluetoothState()
{
	return bluetoothState;
}

int InputState::SetIsLastState()
{
	isLastState = true;
	if(lastState != nullptr)
		delete lastState;
	lastState = nullptr;
	return 0;
}
