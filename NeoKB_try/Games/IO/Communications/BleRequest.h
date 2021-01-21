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
		/// ��s��raw message��i�h
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



		/* ------------- BleBinaryRequest�Ϊ��x�s�ɮת�map ------------- */

		class BleBinaryRequestFileSegmentMap;


		/* ------------- BleRequestMethod ------------- */

		class BleRequestMethod {
		public:

			virtual int PerformAndWait(BleRequest* thisRequest) = 0;

		};

		/// <summary>
		/// post text request������ʧ@
		/// </summary>
		class PostTextBleRequestMethod : public BleRequestMethod {

		public:

			/// <summary>
			/// ���Φ^�Ǫ��غc�l(no ack) 
			/// </summary>
			PostTextBleRequestMethod(MeteoBluetoothMessage* pMessage);

			/// <summary>
			/// �n�^�Ǫ��غc�l
			/// </summary>
			PostTextBleRequestMethod(MeteoBluetoothMessage* pMessage, MeteoCommand aCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);

			int AddOnAck(BleRequest* thisRequest, MtoObject * callableObject, function<int(json)> callback, string name = "HandleAck");

		protected:

			/// <summary>
			/// �o�ӦbRequest�̭�new delete�A
			/// </summary>
			MeteoBluetoothMessage* postMessage;

			MeteoCommand ackCommand = MeteoCommand::None;

			bool isNeedCheckAck = true;

			ActionList<int, json> onAck;

		};

		/// <summary>
		/// post binary request������ʧ@
		/// </summary>
		class PostBinaryBleRequestMethod : public BleRequestMethod {

			/// <summary>
			/// ��̫�@�ӱ׽u�᭱���r
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
		/// get text request������ʧ@
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
		/// get binary request������ʧ@
		/// </summary>
		class GetBinaryBleRequestMethod : public BleRequestMethod {

		public:

			/// <summary>
			/// ��Ƨ���m�̫᭱���n�[�W�׽u
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
			/// ��Ƨ���m�A�̫᭱���n�[�W�׽u
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
		/// �Τ@��map����ble���쪺�ɮפ��q�s�����@�ӧ��㪺�ɮ�
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
			/// ���|close fstream�A���槹���٭n�ۤv�hclose stream
			/// </summary>
			int WriteFile(fstream* fStream);

			/// <summary>
			/// ���|close fstream�A���槹���٭n�ۤv�hclose stream
			/// </summary>
			int ReadFile(fstream* fStream);

		};

	};

}}}

#endif