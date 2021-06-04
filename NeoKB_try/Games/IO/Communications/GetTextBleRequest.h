#ifndef GET_TEXT_BLE_REQUEST_H
#define GET_TEXT_BLE_REQUEST_H

#include "BleRequest.h"

namespace Games {
namespace IO{
namespace Communications{

	class GetTextBleRequest : public BleRequest {

	public:

		/// <summary>
		/// override��
		/// </summary>
		GetTextBleRequest();

		/// <summary>
		/// �����ϥΥ�
		/// </summary>
		GetTextBleRequest(MeteoBluetoothMessage* gMessage, MeteoCommand rCommand);

		int AddOnFail(MtoObject * callableObject, function<int(json)> callback, string name = "HandleFail");

		int AddOnSuccess(MtoObject * callableObject, function<int(json)> callback, string name = "HandleSuccess");

	protected:

		virtual int fail(exception* e);

		virtual int success();

		/// <summary>
		/// get text request������ʧ@
		/// </summary>
		class GetTextBleRequestMethod : public BleRequestMethod {

		public:

			/// <summary>
			/// 
			/// </summary>
			GetTextBleRequestMethod(MeteoBluetoothMessage* gMessage, MeteoCommand rCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);

			virtual BleRequestMethodType GetMethodType();

			virtual int Fail(BleRequest* thisRequest);

			virtual int Success(BleRequest* thisRequest);

			string GetReturnText();

			json GetReturnJson();

			int AddOnFail(MtoObject * callableObject, function<int(json)> callback, string name = "HandleFail");

			int AddOnSuccess(MtoObject * callableObject, function<int(json)> callback, string name = "HandleSuccess");

		protected:

			MeteoBluetoothMessage* getMessage;

			MeteoCommand returnCommand = MeteoCommand::None;

			string returnText = "";

			ActionList<int, json> onSuccess;

			ActionList<int, json> onFail;

		};
	};

}}}



#endif