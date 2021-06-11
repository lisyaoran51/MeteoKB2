#ifndef METEO_BLUETOOTH_PHONE_V2_H
#define METEO_BLUETOOTH_PHONE_V2_H


#include "../../Util/DataStructure/ActionList.h"
#include "../../Framework/Allocation/Hierachal/MtoObject.h"
#include "PacketConverter.h"
#include "../../Games/Output/Bluetooths/MeteoBluetoothMessage.h"
#include "../../Framework/Input/InputState.h"
#include <mutex>
#include <vector>
#include "Gatt/GattServer.h"
#include "../../Framework/Threading/SimpleThread.h"



using namespace Framework::Allocation::Hierachal;
using namespace Framework::Input;
using namespace std;
using namespace Util::DataStructure;
using namespace Games::Output::Bluetooths;
using namespace Desktop::Devices::Gatt;
using namespace Framework::Threading;



namespace Desktop {
namespace Devices {

	
	class MeteoBluetoothPhoneV2 : public SimpleThread {

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

		int AddOnConnect(MtoObject * callableObject, function<int()> callback, string name = "MeteoBluetoothPhoneV2::HandleConnect");

		int AddOnDisconnect(MtoObject * callableObject, function<int()> callback, string name = "MeteoBluetoothPhoneV2::HandleDisconnect");

	protected:

		bool isConnected = false;

		bool isFirstPacketSent = false;

		mutable mutex bluetoothStateMutex;

		mutable mutex inputByteMutex;

		vector<pair<const char*, int>> inputBytes;

		InputState* bluetoothState = nullptr;

		PacketConverter<MeteoCommand>* packetConverter = nullptr;

		vector<BluetoothMessage*> outputMessages;

		ActionList<int> onConnect;

		ActionList<int> onDisconnect;

		bool getIsReady();

		int work();

		int pushBluetoothState(BluetoothMessage* btMessage);

		int handleNewPacket(const char* packet, int length);

		int ConvertPacketToMessage(const char* packet, int length);

		int setMtu(int mtu);

	};


}
}


#endif