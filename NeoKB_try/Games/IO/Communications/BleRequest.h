#ifndef BLE_REQUEST_H
#define BLE_REQUEST_H

#include "../../../Framework/IO/Communications/CommunicationRequest.h"
#include <exception>
#include "../../Input/Commands/MeteoBluetoothCommand.h"
#include "BleRequestException.h"


using namespace std;
using namespace Framework::IO::Communications;
using namespace Games::Input::Commands;


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

	protected:


		BleRequestMethodType requestMethodType = BleRequestMethodType::None;

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
			PostTextBleRequestMethod(MeteoBluetoothCommand* pMessage);

			/// <summary>
			/// 要回傳的建構子
			/// </summary>
			PostTextBleRequestMethod(MeteoBluetoothCommand* pMessage, MeteoCommand aCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);

			int AddOnAck(MtoObject * callableObject, function<int(json)> callback, string name = "HandleAck");

		protected:

			/// <summary>
			/// 這個在Request裡面new delete，
			/// </summary>
			MeteoBluetoothCommand* postMessage;

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

		protected:

			string filePath = "";
			MeteoCommand transferCommand = MeteoCommand::None;
			MeteoCommand finishCommand = MeteoCommand::None;
			MeteoCommand requestRetransferCommand = MeteoCommand::None;
			MeteoCommand ackFinishCommand = MeteoCommand::None;

			BleBinaryRequestFileSegmentMap* fileMap = nullptr;

		};

		/// <summary>
		/// get text request的執行動作
		/// </summary>
		class GetTextBleRequestMethod : public BleRequestMethod {

		public:

			/// <summary>
			/// 
			/// </summary>
			GetTextBleRequestMethod(MeteoBluetoothCommand& gMessage, MeteoCommand rCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);

			string GetReturnText();

			json GetReturnJson();

		protected:

			MeteoBluetoothCommand& getMessage;

			MeteoCommand returnCommand = MeteoCommand::None;

			string returnText = "";

		};

		/// <summary>
		/// get binary request的執行動作
		/// </summary>
		class GetBinaryBleRequestMethod : public BleRequestMethod {

		public:

			/// <summary>
			/// 資料夾位置最後面部要加上斜線
			/// </summary>
			GetBinaryBleRequestMethod(string dPath,
				MeteoBluetoothCommand& gMessage,
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
			MeteoBluetoothCommand& getMessage;
			MeteoCommand getCommand = MeteoCommand::None;
			MeteoCommand transferCommand = MeteoCommand::None;
			MeteoCommand finishCommand = MeteoCommand::None;
			MeteoCommand requestRetransferCommand = MeteoCommand::None;
			MeteoCommand ackFinishCommand = MeteoCommand::None;

			string fileName = "";

			BleBinaryRequestFileSegmentMap* fileSegmentMap = nullptr;

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
			int WriteFile(fstream* fStream);
			int ReadFile(fstream* fStream);

		};

	};

}}}

#endif