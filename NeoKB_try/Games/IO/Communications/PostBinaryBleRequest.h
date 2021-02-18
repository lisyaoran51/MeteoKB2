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

		int AddOnFinish(MtoObject * callableObject, function<int()> callback, string name = "HandleFinish");


	protected:
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
		};
	};

}}}



#endif