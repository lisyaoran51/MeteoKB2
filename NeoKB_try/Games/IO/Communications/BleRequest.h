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
			PostTextBleRequestMethod(MeteoBluetoothCommand* pMessage);

			/// <summary>
			/// �n�^�Ǫ��غc�l
			/// </summary>
			PostTextBleRequestMethod(MeteoBluetoothCommand* pMessage, MeteoCommand aCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);

			int AddOnAck(MtoObject * callableObject, function<int(json)> callback, string name = "HandleAck");

		protected:

			/// <summary>
			/// �o�ӦbRequest�̭�new delete�A
			/// </summary>
			MeteoBluetoothCommand* postMessage;

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

		protected:

			string filePath = "";
			MeteoCommand transferCommand = MeteoCommand::None;
			MeteoCommand finishCommand = MeteoCommand::None;
			MeteoCommand requestRetransferCommand = MeteoCommand::None;
			MeteoCommand ackFinishCommand = MeteoCommand::None;

			BleBinaryRequestFileSegmentMap* fileMap = nullptr;

		};

		/// <summary>
		/// get text request������ʧ@
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
		/// get binary request������ʧ@
		/// </summary>
		class GetBinaryBleRequestMethod : public BleRequestMethod {

		public:

			/// <summary>
			/// ��Ƨ���m�̫᭱���n�[�W�׽u
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
			/// ��Ƨ���m�A�̫᭱���n�[�W�׽u
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
			int WriteFile(fstream* fStream);
			int ReadFile(fstream* fStream);

		};

	};

}}}

#endif