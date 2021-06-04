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
		/// �o�ӥi��Τ���A�]�����n�ɮת��ɭԷ|�D�ʭn�D
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
		/// ���ɤ@��request�i�H�P�ɳQwifi�Bble�Bbt���h��communication component����A�o��request�n�������ثe�i��Ϊ�component�����ǡA�ۤv��w�n�έ���component
		/// �M��A����
		/// </summary>
		//virtual int ChooseCommunicationComponentToPerform();

		int AddOnFail(MtoObject * callableObject, function<int(string)> callback, string name = "HandleFail");

		int AddOnSuccess(MtoObject * callableObject, function<int(string)> callback, string name = "HandleSuccess");


	protected:


		virtual int fail(exception* e);

		virtual int success();

		/// <summary>
		/// post binary request������ʧ@
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