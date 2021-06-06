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

	DisplayDevice* displayDevice = new MeteoBitbangDisplayDevice(48, 16);// ����A��

	MeteoPacketConverterV2* packetConverter = new MeteoPacketConverterV2();
	MeteoBluetoothPhoneV2* bluetoothPhone = new MeteoBluetoothPhoneV2(packetConverter);
	BluetoothDevice* bluetoothDevice = new MeteoBluetoothDevice(bluetoothPhone);

	//MeteoPanelBoardV1* meteoPanelBoard = new MeteoPanelBoardV1(7); // i2c address�]��7
	MeteoMcuV1* meteoMcu = new MeteoMcuV1(0x15);
	KeyboardDevice* keyboardDevice = new MeteoKeyboardDevice(meteoMcu);
	PanelDevice* panelDevice = new MeteoPanelDevice(meteoMcu);



	// bt�Mpanel���P�ɦ�input�Moutput�S�ʡA���Ȯɧ�L�̳��\input
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
	// ����i��n����desktop storage?
	Storage* s = new PlatformStorage(sName);
	s->Initialize();
	return s;

}