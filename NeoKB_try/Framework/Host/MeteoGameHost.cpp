#include "MeteoGameHost.h"

#include "../../Desktop/Devices/MeteoBluetoothDevice.h"
#include "../../Desktop/Devices/MeteoDisplayDevice.h"
#include "../../Desktop/Devices/MeteoKeyboardDevice.h"
#include "../../Desktop/Devices/MeteoPanelDevice.h"
#include "../Input/Handler/BluetoothInputHandler.h"
#include "../Input/Handler/KeyboardInputHandler.h"
#include "../Input/Handler/PanelInputHandler.h"



using namespace Framework::Host;
using namespace Framework::Devices;
using namespace Framework::Input::Handler;
using namespace Desktop::Devices;



MeteoGameHost::MeteoGameHost()
{
	
}

int MeteoGameHost::setupMainInterface()
{
	mainInterface = new MainInterface();

	DisplayDevice* displayDevice = new MeteoDisplayDevice();
	BluetoothDevice* bluetoothDevice = new MeteoBluetoothDevice();
	KeyboardDevice* keyboardDevice = new MeteoKeyboardDevice();
	PanelDevice* panelDevice = new MeteoPanelDevice();

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
