#include "MeteoGameHost.h"

#include "../../Desktop/Devices/MeteoBluetoothDevice.h"
#include "../../Desktop/Devices/MeteoDisplayDevice.h"
#include "../../Desktop/Devices/MeteoKeyboardDevice.h"
#include "../../Desktop/Devices/MeteoPanelDevice.h"
#include "../Input/Handler/BluetoothInputHandler.h"
#include "../Input/Handler/KeyboardInputHandler.h"
#include "../Input/Handler/PanelInputHandler.h"
#include "../IO/PlatformStorage.h"
#include "../Output/OutputManager.h"
#include "../../RulesetMeteor/Output/Panels/SustainPedalLightRingOutputer.h"



using namespace Framework::Host;
using namespace Framework::Devices;
using namespace Framework::Input::Handler;
using namespace Desktop::Devices;
using namespace Framework::IO;
using namespace Framework::Output;
using namespace Meteor::Output::Panels;



MeteoGameHost::MeteoGameHost(): RegisterType("MeteoGameHost")
{
	
}

int MeteoGameHost::setupMainInterface()
{
	LOG(LogLevel::Info) << "MeteoGameHost::setupMainInterface() : Creating devices.";

	mainInterface = new MainInterface();
	mainInterface->Initialize();

	DisplayDevice* displayDevice = new MeteoDisplayDevice(48, 16);// 之後再改
	BluetoothDevice* bluetoothDevice = new MeteoBluetoothDevice();

	MeteoPanelBoardV1* meteoPanelBoard = new MeteoPanelBoardV1(7); // i2c address設為7
	KeyboardDevice* keyboardDevice = new MeteoKeyboardDevice(meteoPanelBoard);
	PanelDevice* panelDevice = new MeteoPanelDevice(meteoPanelBoard);

	// bt和panel都同時有input和output特性，先暫時把他們都擺input
	mainInterface->RegisterInputDevice(bluetoothDevice);
	mainInterface->RegisterInputDevice(keyboardDevice);
	mainInterface->RegisterInputDevice(panelDevice);
	mainInterface->RegisterOutputDevice(displayDevice);


	return 0;
}

int MeteoGameHost::SetupOutputManager(OutputManager * oManager)
{
	OutputComponent* panelOutputer = new SustainPedalLightRingOutputer();
	panelOutputer->SetupPeripheral(mainInterface);
	oManager->AddItem(panelOutputer);

	return 0;
}

vector<InputHandler*>* MeteoGameHost::createAvailableInputHandlers()
{
	LOG(LogLevel::Info) << "MeteoGameHost::createAvailableInputHandlers() : create available input handlers.";

	vector<InputHandler*>* handlers = new vector<InputHandler*>();
	handlers->push_back(new KeyboardInputHandler());
	handlers->push_back(new PanelInputHandler());
	handlers->push_back(new BluetoothInputHandler());

	return handlers;
}

Storage* MeteoGameHost::getStorage(string sName) 
{
	Storage* s = new PlatformStorage(sName);
	s->Initialize();
	return s;

}