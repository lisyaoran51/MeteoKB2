#ifndef GET_BINARY_BLE_REQUEST_HANDLER_H
#define GET_BINARY_BLE_REQUEST_HANDLER_H

#include "BleRequest.h"
#include "../../../Util/DataStructure/FileSegmentMap.h"


using namespace Util::DataStructure;


namespace Games {
namespace IO{
namespace Communications{

	class GetBinaryBleRequestHandler : public BleRequest {

	public:

		GetBinaryBleRequestHandler(string dPath,
			string fName,
			MeteoCommand gCommand,
			MeteoCommand ackGetCommand,
			MeteoCommand tCommand,
			MeteoCommand aTransferCommand,
			MeteoCommand fCommand,
			MeteoCommand rRetransferCommand,
			MeteoCommand aFinishCommand);

		int AddOnSuccess(MtoObject * callableObject, function<int(string)> callback, string name = "HandleSuccess");

		int AddOnSuccess(ActionList<int, string>* callback);

		int AddOnFail(MtoObject * callableObject, function<int(string)> callback, string name = "HandleFail");

		int AddOnFail(ActionList<int, string>* callback);

	protected:

		virtual int fail(exception* e);

		virtual int success();

		/// <summary>
		/// get binary request的執行動作
		/// </summary>
		class GetBinaryBleRequestHandlerMethod : public BleRequestMethod {

		public:

			/// <summary>
			/// 資料夾位置最後面部要加上斜線
			/// </summary>
			GetBinaryBleRequestHandlerMethod(string dPath,
				string fName,
				MeteoCommand gCommand,
				MeteoCommand aGetCommand,
				MeteoCommand tCommand,
				MeteoCommand aTransferCommand,
				MeteoCommand fCommand,
				MeteoCommand rRetransferCommand,
				MeteoCommand aFinishCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);

			virtual int Fail(BleRequest* thisRequest);

			virtual int Success(BleRequest* thisRequest);

			virtual BleRequestMethodType GetMethodType();
			
			int AddOnFail(MtoObject * callableObject, function<int(string)> callback, string name = "HandleFail");

			int AddOnFail(ActionList<int, string>* actionsOnFinish);

			int AddOnSuccess(MtoObject * callableObject, function<int(string)> callback, string name = "HandleSuccess");

			int AddOnSuccess(ActionList<int, string>* callback);

		protected:

			/// <summary>
			/// 資料夾位置，最後面不要加上斜線
			/// </summary>
			string directoryPath = "";
			string fileName = "";
			MeteoCommand getCommand = MeteoCommand::None;
			MeteoCommand ackGetCommand = MeteoCommand::None;
			MeteoCommand transferCommand = MeteoCommand::None;
			MeteoCommand ackTransferCommand = MeteoCommand::None;
			MeteoCommand finishCommand = MeteoCommand::None;
			MeteoCommand requestRetransferCommand = MeteoCommand::None;
			MeteoCommand ackFinishCommand = MeteoCommand::None;

			FileSegmentMap* fileSegmentMap = nullptr;

			bool isTransferFinished = false;
			bool isAckedTransferFinished = false;

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