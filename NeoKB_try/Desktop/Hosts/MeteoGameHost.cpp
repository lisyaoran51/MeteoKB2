#include "MeteoGameHost.h"

#include "../Devices/MeteoBluetoothDevice.h"
#include "../Devices/MeteoBitbangDisplayDevice.h"
#include "../Devices/MeteoKeyboardDevice.h"
#include "../Devices/MeteoPanelDevice.h"
#include "../../Framework/IO/PlatformStorage.h"
#include "../Devices/MeteoPacketConverterV2.h"



using namespace Desktop::Hosts;
using namespace Framework::Hosts;
using namespace Desktop::Devices;
using namespace Framework::IO;



MeteoGameHost::MeteoGameHost(): RegisterType("MeteoGameHost")
{
	
}

int MeteoGameHost::setupMainInterface()
{
	LOG(LogLevel::Info) << "MeteoGameHost::setupMainInterface() : Creating devices.";

	mainInterface = new MainInterface();
	mainInterface->Initialize();

	DisplayDevice* displayDevice = new MeteoBitbangDisplayDevice(48, 16);// 之後再改

	MeteoPacketConverterV2* packetConverter = new MeteoPacketConverterV2();
	MeteoBluetoothPhoneV2* bluetoothPhone = new MeteoBluetoothPhoneV2(packetConverter);
	BluetoothDevice* bluetoothDevice = new MeteoBluetoothDevice(bluetoothPhone);

	//MeteoPanelBoardV1* meteoPanelBoard = new MeteoPanelBoardV1(7); // i2c address設為7
	MeteoMcuV1* meteoMcu = new MeteoMcuV1(0x15);
	KeyboardDevice* keyboardDevice = new MeteoKeyboardDevice(meteoMcu);
	PanelDevice* panelDevice = new MeteoPanelDevice(meteoMcu);



	// bt和panel都同時有input和output特性，先暫時把他們都擺input
	mainInterface->RegisterInputDevice(bluetoothDevice);
	mainInterface->RegisterInputDevice(keyboardDevice);
	mainInterface->RegisterInputDevice(panelDevice);
	mainInterface->RegisterOutputDevice(bluetoothDevice);
	mainInterface->RegisterOutputDevice(displayDevice);
	mainInterface->RegisterOutputDevice(panelDevice);


	return 0;
}

Storage* MeteoGameHost::getStorage(string sName) 
{
	// 之後可能要換成desktop storage?
	Storage* s = new PlatformStorage(sName);
	s->Initialize();
	return s;

}