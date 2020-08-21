#ifndef METEO_BLUETOOTH_PHONE_V1_H
#define METEO_BLUETOOTH_PHONE_V1_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>
#include "../../Util/DataStructure/ActionList.h"

#include "PacketConverter.h"
#include "../../Games/Input/Commands/MeteoBluetoothCommand.h"
#include "../../Framework/Input/InputState.h"
#include <mutex>
#include <vector>



using namespace Games::Input::Commands;
using namespace Framework::Input;
using namespace std;
using namespace Util::DataStructure;




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

		/// <summary>
		/// 寫入sm檔時的callback
		/// </summary>
		int AddOnStartWritingSmFile(function<int(string)> callback);
		int AddOnWriteSmFileSuccess(function<int(string)> callback);

		/// <summary>
		/// 寫入新音色時的callback
		/// </summary>
		int AddOnStartWritingSoundFilePackage(function<int(string)> callback);
		int AddOnWriteSoundFilePackageSuccess(function<int(string)> callback);

		/// <summary>
		/// 寫入新主程式時的callback
		/// </summary>
		int AddOnStartWritingProgram(function<int(string)> callback);
		int AddOnWriteProgramSuccess(function<int(string)> callback);

	protected:

		bool isConnected = false;

		mutable mutex bluetoothStateMutex;

		InputState* bluetoothState = nullptr;

		PacketConverter<MeteoCommand>* packetConverter = nullptr;

		vector<MeteoBluetoothCommand*> outputMessages;

		ActionList<int, string> onStartWritingSmFile;
		ActionList<int, string> onWriteSmFileSuccess;

		ActionList<int, string> onStartWritingSoundFilePackage;
		ActionList<int, string> onWriteSoundFilePackageSuccess;

		ActionList<int, string> onStartWritingProgram;
		ActionList<int, string> onWriteProgramSuccess;

		int work();

		int readBluetooth();

		int writeBluetooth();

		int pushBluetoothState(BluetoothCommand* btCommand);
	};


}}


#endif