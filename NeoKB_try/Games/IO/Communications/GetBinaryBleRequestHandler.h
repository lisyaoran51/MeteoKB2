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
		/// get binary request������ʧ@
		/// </summary>
		class GetBinaryBleRequestHandlerMethod : public BleRequestMethod {

		public:

			/// <summary>
			/// ��Ƨ���m�̫᭱���n�[�W�׽u
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
			/// ��Ƨ���m�A�̫᭱���n�[�W�׽u
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
			/// ��X�@��file sgegment�H��h�[�S������Ack�A�N�|�۰ʥ�U�@��file segment
			/// </summary>
			double sendFileSegmentTimeout = 0.5; // 0.1

			vector<int> retransferOrders;

			ActionList<int, string> onFail;
			ActionList<int, string> onSuccess;

		};

	};

}}}



#endif