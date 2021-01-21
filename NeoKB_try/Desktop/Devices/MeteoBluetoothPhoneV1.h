#ifndef METEO_BLUETOOTH_PHONE_V1_H
#define METEO_BLUETOOTH_PHONE_V1_H

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



using namespace Games::Input::Commands;
using namespace Framework::Input;
using namespace std;
using namespace Util::DataStructure;
using namespace Games::Output::Bluetooths;




namespace Desktop {
namespace Devices {

	
	class MeteoBluetoothPhoneV1 {

		bool exitRequested = false;

		sdp_session_t *session = nullptr;

		int client = -1;

		char bufferIn[32768] = { 0 };

		char bufferOut[1024] = { 0 };

		bool lastRunReceived = false;

		bool lastRunSended = false;

		/// <summary>
		/// 輸出檔案之後要等幾個回合才開始詢問收到了沒
		/// </summary>
		int OutputMessageToRewriteCountdown = 3;

		sdp_session_t *register_service(int port);

		int init_server(int port);

	public:

		MeteoBluetoothPhoneV1(PacketConverter<MeteoCommand>* pConverter);

		int Initialize();

		InputState* GetBluetoothState();

		int PushOutputMessage(OutputMessage* outputMessage);

		/// <summary>
		/// 寫入sm檔時的callback
		/// </summary>
		template<class _Type>
		int AddOnStartWritingSmFile(_Type* callableObject, function<int(string)> callback, string name) {
			onStartWritingSmFile.Add(callableObject, callback, name);
			return 0;
		}
		template<class _Type>
		int AddOnWriteSmFileSuccess(_Type* callableObject, function<int(string)> callback, string name) {
			onWriteSmFileSuccess.Add(callableObject, callback, name);
			return 0;
		}

		/// <summary>
		/// 寫入新音色時的callback
		/// </summary>
		template<class _Type>
		int AddOnStartWritingSoundFilePackage(_Type* callableObject, function<int(string)> callback, string name) {
			onStartWritingSoundFilePackage.Add(callableObject, callback, name);
			return 0;
		}
		template<class _Type>
		int AddOnWriteSoundFilePackageSuccess(_Type* callableObject, function<int(string)> callback, string name) {
			onWriteSoundFilePackageSuccess.Add(callableObject, callback, name);
			return 0;
		}

		/// <summary>
		/// 寫入新主程式時的callback
		/// </summary>
		template<class _Type>
		int AddOnStartWritingProgram(_Type* callableObject, function<int(string)> callback, string name) {
			onStartWritingProgram.Add(callableObject, callback, name);
			return 0;
		}
		template<class _Type>
		int AddOnWriteProgramSuccess(_Type* callableObject, function<int(string)> callback, string name) {
			onWriteProgramSuccess.Add(callableObject, callback, name);
			return 0;
		}

	protected:

		bool isConnected = false;

		mutable mutex bluetoothStateMutex;

		InputState* bluetoothState = nullptr;

		PacketConverter<MeteoCommand>* packetConverter = nullptr;

		vector<BluetoothMessage*> outputMessages;

		/// <summary>
		/// 看有沒有掉封包，掉了要再重傳。pair前面的int是等待迴圈數，會從3開始倒數，每write bluetooth時-1。扣到0的時候才開始rewrite。
		/// 是用來避免手機還沒接收完封包，琴就一直傳訊息問手機收到沒，所以先等3輪再開始問。
		/// </summary>
		//vector<pair<int, MeteoOutputFileBluetoothCommand*>> outputMessagesToRewrite;	// 先不用這個

		ActionList<int, string> onStartWritingSmFile;
		ActionList<int, string> onWriteSmFileSuccess;

		ActionList<int, string> onStartWritingSoundFilePackage;
		ActionList<int, string> onWriteSoundFilePackageSuccess;

		ActionList<int, string> onStartWritingProgram;
		ActionList<int, string> onWriteProgramSuccess;

		int work();

		int readBluetooth();

		int writeBluetooth();

		int pushBluetoothState(BluetoothMessage* btMessage);

		int handleNewPacket(char* packet, int length);

	};


}}


#endif