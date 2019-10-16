#include "MeteoGameHost.h"

#include "../Devices/BluetoothDevice.h"
#include "../Devices/DisplayDevice.h"
#include "../Devices/KeyboardDevice.h"
#include "../Devices/PanelDevice.h"
#include "../Input/Handler/BluetoothInputHandler.h"
#include "../Input/Handler/KeyboardInputHandler.h"
#include "../Input/Handler/PanelInputHandler.h"



using namespace Framework::Platforms;
using namespace Framework::Devices;
using namespace Framework::Input::Handler;


MeteoGameHost::MeteoGameHost()
{
	
}

int MeteoGameHost::setupMainInterface()
{
	mainInterface = new MainInterface();

	DisplayDevice* displayDevice = new MeteoDisplayDevice();
	BluetoothDevice* bluetoothDevice = new MeteoBluetoothDevice();
	KeyboardDevice* keyboardDevice = new MeteoKeyboardDevice();
	PanelDevice* panelDevice = new MeteoKeyboardDevice();

	// bt和panel都同時有input和output特性，先暫時把他們都擺input
	mainInterface->RegisterInputDevice(bluetoothDevice);
	mainInterface->RegisterInputDevice(keyboardDevice);
	mainInterface->RegisterInputDevice(panelDevice);
	mainInterface->RegisterOutputDevice(displayDevice);


	return 0;
}

vector<InputHandler*>* MeteoGameHost::createAvailableInputHandlers()
{
	vector<InputHandler*>* handlers = new vector<InputHandler*>();
	handlers->push_back(new KeyboardInputHandler());
	handlers->push_back(new PanelInputHandler());
	handlers->push_back(new BluetoothInputHandler());

	return handlers;
}
