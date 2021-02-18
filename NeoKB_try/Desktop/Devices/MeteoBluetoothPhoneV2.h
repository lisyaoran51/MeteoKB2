#ifndef METEO_BLUETOOTH_PHONE_V2_H
#define METEO_BLUETOOTH_PHONE_V2_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>
#include "../../Util/DataStructure/ActionList.h"

#include "PacketConverter.h"
#include "../../Games/Output/Bluetooths/MeteoBluetoothMessage.h"
#include "../../Framework/Input/InputState.h"
#include <mutex>
#include <vector>




using namespace Framework::Input;
using namespace std;
using namespace Util::DataStructure;
using namespace Games::Output::Bluetooths;




namespace Desktop {
namespace Devices {

	
	class MeteoBluetoothPhoneV2 {

		bool exitRequested = false;

		sdp_session_t *session = nullptr;

		int client = -1;

		char bufferIn[32768] = { 0 };

		char bufferOut[1024] = { 0 };

		bool lastRunReceived = false;

		bool lastRunSended = false;

		sdp_session_t *register_service(int port);

		int init_server(int port);

	public:

		MeteoBluetoothPhoneV2(PacketConverter<MeteoCommand>* pConverter);

		int Initialize();

		InputState* GetBluetoothState();

		int PushOutputMessage(OutputMessage* outputMessage);

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

		int handleNewPacket(char* packet, int length);

	};


}}


#endif