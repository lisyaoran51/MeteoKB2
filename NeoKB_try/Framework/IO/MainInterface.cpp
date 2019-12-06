#include "MainInterface.h"

#include "Display.h"
#include "Keyboard.h"
#include "Panel.h"
#include "BluetoothPhone.h"

#include "../Devices/InputDevice.h"
#include "../Devices/OutputDevice.h"


using namespace Framework::IO;
using namespace Framework::Devices;



MainInterface::MainInterface() : RegisterType("MainInterface")
{

}

int MainInterface::Initialize()
{
	display = new Display();
	keyboard = new Keyboard();
	panel = new Panel();
	phone = new BluetoothPhone();


	peripherals.push_back(display);
	peripherals.push_back(keyboard);
	peripherals.push_back(panel);
	peripherals.push_back(phone);

	return 0;
}

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

vector<Peripheral*>* MainInterface::GetPeripherals()
{
	return &peripherals;
}

int MainInterface::RegisterInputDevice(InputDevice * inputDevice)
{
	inputDevice->Register(this);
	inputDevices.push_back(inputDevice);
	devices.push_back(inputDevice);
	return 0;
}

int MainInterface::RegisterOutputDevice(OutputDevice * outputDevice)
{
	outputDevice->Register(this);
	outputDevices.push_back(outputDevice);
	devices.push_back(outputDevice);

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
