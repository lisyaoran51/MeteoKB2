#ifndef BLE_REQUEST_H
#define BLE_REQUEST_H

#include "../../../Framework/IO/Communications/CommunicationRequest.h"
#include <exception>
#include "../../Output/Bluetooths/MeteoBluetoothMessage.h"
#include "BleRequestException.h"
#include "../../../ThirdParty/json/json.hpp"


using namespace std;
using namespace Framework::IO::Communications;
using namespace Games::Output::Bluetooths;

using json = nlohmann::json;


namespace Games {
namespace IO{
namespace Communications{

	enum class BleRequestMethodType {
		None,
		PostText,
		PostBinary,
		GetText,
		GetBinary,
	};


	class BleRequest : virtual public CommunicationRequest {

	public:

		virtual int Perform(CommunicationComponent* cComponent);

		/// <summary>
		/// 把新的raw message丟進去
		/// </summary>
		int PushInputRawMessage(MeteoBluetoothMessage* rawMessage);

	protected:


		BleRequestMethodType requestMethodType = BleRequestMethodType::None;

		deque<MeteoBluetoothMessage*> inputRawMessages;

		deque<MeteoBluetoothMessage*> inputRawMessagesBuffer;

		mutable mutex rawMessageMutex;

		mutable mutex rawMessageBufferMutex;


		class BleRequestMethod;

		BleRequestMethod* requestMethod = nullptr;



		/* ------------- BleBinaryRequest用的儲存檔案的map ------------- */

		class BleBinaryRequestFileSegmentMap;


		/* ------------- BleRequestMethod ------------- */

		class BleRequestMethod {
		public:

			virtual int PerformAndWait(BleRequest* thisRequest) = 0;

		};

		/// <summary>
		/// post text request的執行動作
		/// </summary>
		class PostTextBleRequestMethod : public BleRequestMethod {

		public:

			/// <summary>
			/// 不用回傳的建構子(no ack) 
			/// </summary>
			PostTextBleRequestMethod(MeteoBluetoothMessage* pMessage);

			/// <summary>
			/// 要回傳的建構子
			/// </summary>
			PostTextBleRequestMethod(MeteoBluetoothMessage* pMessage, MeteoCommand aCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);

			int AddOnAck(BleRequest* thisRequest, MtoObject * callableObject, function<int(json)> callback, string name = "HandleAck");

		protected:

			/// <summary>
			/// 這個在Request裡面new delete，
			/// </summary>
			MeteoBluetoothMessage* postMessage;

			MeteoCommand ackCommand = MeteoCommand::None;

			bool isNeedCheckAck = true;

			ActionList<int, json> onAck;

		};

		/// <summary>
		/// post binary request的執行動作
		/// </summary>
		class PostBinaryBleRequestMethod : public BleRequestMethod {

			/// <summary>
			/// 抓最後一個斜線後面的字
			/// </summary>
			string getFileNameFromPath();

		public:

			/// <summary>
			/// 
			/// </summary>
			PostBinaryBleRequestMethod(string fPath,
				MeteoCommand tCommand,
				MeteoCommand fCommand,
				MeteoCommand rRetransferCommand,
				MeteoCommand aFinishCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);


			int AddOnFinish(MtoObject * callableObject, function<int()> callback, string name = "HandleFinish");

		protected:

			string filePath = "";
			MeteoCommand transferCommand = MeteoCommand::None;
			MeteoCommand finishCommand = MeteoCommand::None;
			MeteoCommand requestRetransferCommand = MeteoCommand::None;
			MeteoCommand ackFinishCommand = MeteoCommand::None;

			BleBinaryRequestFileSegmentMap* fileMap = nullptr;

			vector<int> retransferOrders;

			ActionList<int> onFinish;
		};

		/// <summary>
		/// get text request的執行動作
		/// </summary>
		class GetTextBleRequestMethod : public BleRequestMethod {

		public:

			/// <summary>
			/// 
			/// </summary>
			GetTextBleRequestMethod(MeteoBluetoothMessage* gMessage, MeteoCommand rCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);

			string GetReturnText();

			json GetReturnJson();

		protected:

			MeteoBluetoothMessage* getMessage;

			MeteoCommand returnCommand = MeteoCommand::None;

			string returnText = "";

			ActionList<int, json> onReturn;

		};

		/// <summary>
		/// get binary request的執行動作
		/// </summary>
		class GetBinaryBleRequestMethod : public BleRequestMethod {

		public:

			/// <summary>
			/// 資料夾位置最後面部要加上斜線
			/// </summary>
			GetBinaryBleRequestMethod(string fPath,
				MeteoBluetoothMessage* gMessage,
				MeteoCommand ackGetCommand,
				MeteoCommand tCommand,
				MeteoCommand fCommand,
				MeteoCommand rRetransferCommand,
				MeteoCommand aFinishCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);

		protected:

			/// <summary>
			/// 資料夾位置，最後面不要加上斜線
			/// </summary>
			string directoryPath = "";
			string fileName = "";
			MeteoBluetoothMessage* getMessage;
			MeteoCommand getCommand = MeteoCommand::None;
			MeteoCommand ackGetCommand = MeteoCommand::None;
			MeteoCommand transferCommand = MeteoCommand::None;
			MeteoCommand finishCommand = MeteoCommand::None;
			MeteoCommand requestRetransferCommand = MeteoCommand::None;
			MeteoCommand ackFinishCommand = MeteoCommand::None;

			string fileName = "";

			BleBinaryRequestFileSegmentMap* fileSegmentMap = nullptr;

			bool isTransferFinished = false;
			bool isAckedTransferFinished = false;

			ActionList<int> onFinish;
			ActionList<int, json> onAck;

		};


		/* ------------- BleRequestMethod ------------- */

		/// <summary>
		/// 用一個map來讓ble收到的檔案片段連接成一個完整的檔案
		/// </summary>
		class BleBinaryRequestFileSegmentMap {
		public:
			~BleBinaryRequestFileSegmentMap();

			string fileName;
			map<int, pair<char*, int>> fileSegmentMap;
			int segmentSize;
			int segmentAmount;
			bool CheckFullFilled();

			/// <summary>
			/// 部會close fstream，執行完畢還要自己去close stream
			/// </summary>
			int WriteFile(fstream* fStream);

			/// <summary>
			/// 部會close fstream，執行完畢還要自己去close stream
			/// </summary>
			int ReadFile(fstream* fStream);

		};

	};

}}}

#endif