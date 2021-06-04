#ifndef POST_BINARY_BLE_REQUEST_H
#define POST_BINARY_BLE_REQUEST_H

#include "BleRequest.h"
#include "../../../Util/DataStructure/FileSegmentMap.h"

using namespace Util::DataStructure;

namespace Games {
namespace IO{
namespace Communications{

	class PostBinaryBleRequest : public BleRequest {

	public:
		
		/// <summary>
		/// 這個可能用不到，因為對方要檔案的時候會主動要求
		/// </summary>
		PostBinaryBleRequest(string fPath,
			MeteoBluetoothMessage* pMessage,
			MeteoCommand ackPostCommand,
			MeteoCommand tCommand,
			MeteoCommand aTransferCommand,
			MeteoCommand fCommand,
			MeteoCommand rRetransferCommand,
			MeteoCommand aFinishCommand);

		/// <summary>
		/// 有時一個request可以同時被wifi、ble、bt等多種communication component執行，這時request要先接收目前可選用的component有哪些，自己選定要用哪種component
		/// 然後再執行
		/// </summary>
		//virtual int ChooseCommunicationComponentToPerform();

		int AddOnFail(MtoObject * callableObject, function<int(string)> callback, string name = "HandleFail");

		int AddOnSuccess(MtoObject * callableObject, function<int(string)> callback, string name = "HandleSuccess");


	protected:


		virtual int fail(exception* e);

		virtual int success();

		/// <summary>
		/// post binary request的執行動作
		/// </summary>
		class PostBinaryBleRequestMethod : public BleRequestMethod {


		public:

			/// <summary>
			/// 
			/// </summary>
			PostBinaryBleRequestMethod(string fPath,
				MeteoBluetoothMessage* pMessage,
				MeteoCommand aPostCommand,
				MeteoCommand tCommand,
				MeteoCommand aTranferCommand,
				MeteoCommand fCommand,
				MeteoCommand rRetransferCommand,
				MeteoCommand aFinishCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);

			virtual BleRequestMethodType GetMethodType();

			virtual int Fail(BleRequest* thisRequest);

			virtual int Success(BleRequest* thisRequest);

			int AddOnFail(MtoObject * callableObject, function<int(string)> callback, string name = "HandleAck");

			int AddOnSuccess(MtoObject * callableObject, function<int(string)> callback, string name = "HandleFinish");

		protected:

			string filePath = "";
			MeteoBluetoothMessage* postMessage = nullptr;
			MeteoCommand ackPostCommand = MeteoCommand::None;
			MeteoCommand transferCommand = MeteoCommand::None;
			MeteoCommand ackTransferCommand = MeteoCommand::None;
			MeteoCommand finishCommand = MeteoCommand::None;
			MeteoCommand requestRetransferCommand = MeteoCommand::None;
			MeteoCommand ackFinishCommand = MeteoCommand::None;

			FileSegmentMap* fileMap = nullptr;

			/// <summary>
			/// 丟出一個file sgegment以後多久沒有收到Ack，就會自動丟下一個file segment
			/// </summary>
			double sendFileSegmentTimeout = 0.5; // 0.1

			vector<int> retransferOrders;
			ActionList<int, string> onFail;
			ActionList<int, string> onSuccess;
		};
	};

}}}



#endif