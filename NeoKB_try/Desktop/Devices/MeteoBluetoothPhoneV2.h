#ifndef METEO_BLUETOOTH_PHONE_V2_H
#define METEO_BLUETOOTH_PHONE_V2_H


#include "../../Util/DataStructure/ActionList.h"

#include "PacketConverter.h"
#include "../../Games/Output/Bluetooths/MeteoBluetoothMessage.h"
#include "../../Framework/Input/InputState.h"
#include <mutex>
#include <vector>
#include "Gatt/GattServer.h"




using namespace Framework::Input;
using namespace std;
using namespace Util::DataStructure;
using namespace Games::Output::Bluetooths;
using namespace Desktop::Devices::Gatt;



namespace Desktop {
namespace Devices {

	
	class MeteoBluetoothPhoneV2 {

		bool exitRequested = false;

		int client = -1;

		char bufferIn[32768] = { 0 };

		char bufferOut[1024] = { 0 };

		bool lastRunReceived = false;

		bool lastRunSended = false;

		GattServer* gattServer = nullptr;

		int outputBufferThreshold = 5;

		int mtu = 256;

		int maxMtu = 256;

	public:

		MeteoBluetoothPhoneV2(PacketConverter<MeteoCommand>* pConverter);

		int Initialize();

		InputState* GetBluetoothState();

		int PushOutputMessage(BluetoothMessage* outputMessage);

	protected:

		bool isConnected = false;

		mutable mutex bluetoothStateMutex;

		InputState* bluetoothState = nullptr;

		PacketConverter<MeteoCommand>* packetConverter = nullptr;

		vector<BluetoothMessage*> outputMessages;

		int work();

		int readBluetooth();

		int writeBluetooth();

		int pushBluetoothState(BluetoothMessage* btMessage);

		int handleNewPacket(const char* packet, int length);

		int setMtu(int mtu);

	};


}}


#endif