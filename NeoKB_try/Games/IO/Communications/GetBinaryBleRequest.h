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
			MeteoCommand aTransferCommand,
			MeteoCommand fCommand,
			MeteoCommand rRetransferCommand,
			MeteoCommand aFinishCommand);

		/// <summary>
		/// ���ɤ@��request�i�H�P�ɳQwifi�Bble�Bbt���h��communication component����A�o��request�n�������ثe�i��Ϊ�component�����ǡA�ۤv��w�n�έ���component
		/// �M��A����
		/// </summary>
		//virtual int ChooseCommunicationComponentToPerform();

		int AddOnSuccess(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name = "HandleSuccess");

		int AddOnSuccess(ActionList<int, FileSegmentMap*>* callback);

		int AddOnFail(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name = "HandleFail");

		int AddOnFail(ActionList<int, FileSegmentMap*>* callback);

	protected:

		virtual int fail(exception* e);

		virtual int success();

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

			int AddOnSuccess(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name = "HandleSuccess");

			int AddOnSuccess(ActionList<int, FileSegmentMap*>* callback);

			int AddOnFail(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name = "HandleFail");

			int AddOnFail(ActionList<int, FileSegmentMap*>* callback);

		protected:

			/// <summary>
			/// ��Ƨ���m�A�̫᭱���n�[�W�׽u
			/// </summary>
			string directoryPath = "";
			string fileName = "";
			MeteoBluetoothMessage* getMessage;
			MeteoCommand ackGetCommand = MeteoCommand::None;
			MeteoCommand transferCommand = MeteoCommand::None;
			MeteoCommand ackTransferCommand = MeteoCommand::None;
			MeteoCommand finishCommand = MeteoCommand::None;
			MeteoCommand requestRetransferCommand = MeteoCommand::None;
			MeteoCommand ackFinishCommand = MeteoCommand::None;

			FileSegmentMap* fileSegmentMap = nullptr;

			bool isTransferFinished = false;
			bool isAckedTransferFinished = false;

			ActionList<int, FileSegmentMap*> onSuccess;
			ActionList<int, FileSegmentMap*> onFail;

		};

	};

}}}



#endif