#ifndef POST_TEXT_BLE_REQUEST_H
#define POST_TEXT_BLE_REQUEST_H

#include "BleRequest.h"

namespace Games {
namespace IO{
namespace Communications{

	class PostTextBleRequest : public BleRequest {

	public:

		PostTextBleRequest();

		PostTextBleRequest(MeteoBluetoothMessage* pMessage);

		PostTextBleRequest(MeteoBluetoothMessage* pMessage, MeteoCommand aCommand);

		int AddOnFail(MtoObject * callableObject, function<int(json)> callback, string name = "HandleFail");

		int AddOnSuccess(MtoObject * callableObject, function<int(json)> callback, string name = "HandleSuccess");

	protected:

		virtual int fail(exception* e);

		virtual int success();

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

			virtual BleRequestMethodType GetMethodType();

			virtual int Fail(BleRequest* thisRequest);

			virtual int Success(BleRequest* thisRequest);

			string GetAckText();

			json GetAckJson();

			int AddOnFail(MtoObject * callableObject, function<int(json)> callback, string name = "HandleFail");

			int AddOnSuccess(MtoObject * callableObject, function<int(json)> callback, string name = "HandleSuccess");

		protected:

			/// <summary>
			/// �o�ӦbRequest�̭�new delete�A
			/// </summary>
			MeteoBluetoothMessage* postMessage;

			MeteoCommand ackCommand = MeteoCommand::None;

			bool isNeedCheckAck = true;

			string ackText = "";

			ActionList<int, json> onSuccess;

			ActionList<int, json> onFail;

		};
	};

}}}



#endif