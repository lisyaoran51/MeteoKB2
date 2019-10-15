#include "MeteoGameHost.h"

#include "../Devices/BluetoothDevice.h"
#include "../Devices/DisplayDevice.h"
#include "../Devices/KeyboardDevice.h"
#include "../Devices/PanelDevice.h"

using namespace Framework::Platforms;
using namespace Framework::Devices;


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

	// bt�Mpanel���P�ɦ�input�Moutput�S�ʡA���Ȯɧ�L�̳��\input
	mainInterface->RegisterInputDevice(bluetoothDevice);
	mainInterface->RegisterInputDevice(keyboardDevice);
	mainInterface->RegisterInputDevice(panelDevice);
	mainInterface->RegisterOutputDevice(displayDevice);


	return 0;
}
