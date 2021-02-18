#ifndef GET_BINARY_BLE_REQUEST_H
#define GET_BINARY_BLE_REQUEST_H

#include "BleRequest.h"
#include "../../../Util/DataStructure/FileSegmentMap.h"


using namespace Util::DataStructure;


namespace Games {
namespace IO{
namespace Communications{

	class GetBinaryBleRequest : public BleRequest {

	public:

		GetBinaryBleRequest(string fPath,
			MeteoBluetoothMessage* gMessage,
			MeteoCommand ackGetCommand,
			MeteoCommand tCommand,
			MeteoCommand fCommand,
			MeteoCommand rRetransferCommand,
			MeteoCommand aFinishCommand);

		int AddOnFinish(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name = "HandleFinish");

		int AddOnFinish(ActionList<int>* actionsOnFinish);

		int AddOnGetBinarySuccess(MtoObject * callableObject, function<int(string)> callback, string name = "HandleGetSuccess");

	protected:

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
				MeteoCommand aGetCommand,
				MeteoCommand tCommand,
				MeteoCommand fCommand,
				MeteoCommand rRetransferCommand,
				MeteoCommand aFinishCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);

			virtual BleRequestMethodType GetMethodType();

			int AddOnFinish(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name = "HandleFinish");

			int AddOnFinish(ActionList<int, FileSegmentMap*>* actionsOnFinish);

			int AddOnGetBinarySuccess(MtoObject * callableObject, function<int(string)> callback, string name = "HandleGetBinartSuccess");

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

			FileSegmentMap* fileSegmentMap = nullptr;

			bool isTransferFinished = false;
			bool isAckedTransferFinished = false;

			ActionList<int, FileSegmentMap*> onFinish;
			ActionList<int, string> onGetBinarySuccess;
			ActionList<int, json> onAck;

		};

	};

}}}



#endif