#ifndef METEO_BLUETOOTH_PHONE_V1_H
#define METEO_BLUETOOTH_PHONE_V1_H


#include "PacketConverter.h"
#include "../../Games/Input/Commands/MeteoBluetoothCommand.h"
#include "../../Framework/Input/InputState.h"
#include <mutex>
#include <vector>


using namespace Games::Input::Commands;
using namespace Framework::Input;
using namespace std;


namespace Desktop {
namespace Devices {

	
	class MeteoBluetoothPhoneV1 {

		bool exitRequested = false;

		sdp_session_t *session = nullptr;

		int client = -1;

		char bufferIn[32768] = { 0 };

		char bufferOut[32768] = { 0 };

		bool lastRunReceived = false;

		bool lastRunSended = false;

		sdp_session_t *register_service(int port);

		int init_server(int port);

	public:

		MeteoBluetoothPhoneV1(PacketConverter<MeteoCommand>* pConverter);

		InputState* GetBluetoothState();

		int PushOutputMessage(OutputMessage* outputMessage);

	protected:

		bool isConnected = false;

		mutable mutex bluetoothStateMutex;

		InputState* bluetoothState = nullptr;

		PacketConverter<MeteoCommand>* packetConverter = nullptr;

		vector<MeteoBluetoothCommand*> outputMessages;

		int work();

		int readBluetooth();

		int writeBluetooth();

		int pushBluetoothState(BluetoothCommand* btCommand);
	};


}}


#endif