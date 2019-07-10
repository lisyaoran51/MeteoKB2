#include "MainInterface.h"

using namespace Framework::IO;


int MainInterface::ScanInput()
{
	for (int i = 0; i < inputDevices.size(); i++) {
		inputDevices[i]->ScanInput();
	}

	return 0;
}

int MainInterface::ProcessOutput()
{
	for (int i = 0; i < outputDevices.size(); i++) {
		outputDevices[i]->ProcessOutput();
	}

	return 0;
}

int MainInterface::RegisterInputDevice(InputDevice * inputDevice)
{
	inputDevice->Register(this);
	inputDevices.push_back(inputDevice);

	return 0;
}

int MainInterface::RegisterOutputDevice(OutputDevice * outputDevice)
{
	outputDevice->Register(this);
	outputDevices.push_back(outputDevice);

	return 0;
}

int MainInterface::RegisterDisplay(Display * d)
{
	display = d;
	onPeripheralRegister.Trigger(display);
	return 0;
}

Display * MainInterface::GetDisplay()
{
	return display;
}

int MainInterface::RegisterKeyboard(Keyboard * kb)
{
	keyboard = kb;
	onPeripheralRegister.Trigger(kb);
	return 0;
}

Keyboard * MainInterface::GetKeyboard()
{
	return keyboard;
}

int MainInterface::RegisterPanel(Panel * p)
{
	panel = p;
	onPeripheralRegister.Trigger(p);
	return 0;
}

Panel * MainInterface::GetPanel()
{
	return panel;
}

int MainInterface::RegisterBluetoothPhone(BluetoothPhone * btPhone)
{
	phone = btPhone;
	onPeripheralRegister.Trigger(btPhone);
	return 0;
}

BluetoothPhone * MainInterface::GetBluetoothPhone()
{
	return phone;
}
