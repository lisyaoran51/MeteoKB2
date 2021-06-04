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
		/// post text request的執行動作
		/// </summary>
		class PostTextBleRequestMethod : public BleRequestMethod {

		public:

			/// <summary>
			/// 不用回傳的建構子(no ack) 
			/// </summary>
			PostTextBleRequestMethod(MeteoBluetoothMessage* pMessage);

			/// <summary>
			/// 要回傳的建構子
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
			/// 這個在Request裡面new delete，
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