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
		
		PostBinaryBleRequest(string fPath,
			MeteoBluetoothMessage* pMessage,
			MeteoCommand ackPostCommand,
			MeteoCommand tCommand,
			MeteoCommand fCommand,
			MeteoCommand rRetransferCommand,
			MeteoCommand aFinishCommand);

		/// <summary>
		/// 有時一個request可以同時被wifi、ble、bt等多種communication component執行，這時request要先接收目前可選用的component有哪些，自己選定要用哪種component
		/// 然後再執行
		/// </summary>
		//virtual int ChooseCommunicationComponentToPerform();

		int AddOnAck(MtoObject * callableObject, function<int(json)> callback, string name = "HandleAck");

		int AddOnFinish(MtoObject * callableObject, function<int()> callback, string name = "HandleFinish");


	protected:



		virtual int fail(exception& e);

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
				MeteoCommand fCommand,
				MeteoCommand rRetransferCommand,
				MeteoCommand aFinishCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);

			virtual BleRequestMethodType GetMethodType();

			int AddOnAck(MtoObject * callableObject, function<int(json)> callback, string name = "HandleAck");

			int AddOnFinish(MtoObject * callableObject, function<int()> callback, string name = "HandleFinish");

		protected:

			string filePath = "";
			MeteoBluetoothMessage* postMessage = nullptr;
			MeteoCommand ackPostCommand = MeteoCommand::None;
			MeteoCommand transferCommand = MeteoCommand::None;
			MeteoCommand finishCommand = MeteoCommand::None;
			MeteoCommand requestRetransferCommand = MeteoCommand::None;
			MeteoCommand ackFinishCommand = MeteoCommand::None;

			FileSegmentMap* fileMap = nullptr;

			vector<int> retransferOrders;

			ActionList<int> onFinish;
			ActionList<int, string> onPostBinarySuccess;
			ActionList<int, json> onAck;
		};
	};

}}}



#endif