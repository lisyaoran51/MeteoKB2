#ifndef GET_TEXT_BLE_REQUEST_H
#define GET_TEXT_BLE_REQUEST_H

#include "BleRequest.h"

namespace Games {
namespace IO{
namespace Communications{

	class GetTextBleRequest : public BleRequest {

	public:


	protected:
		/// <summary>
		/// get text request的執行動作
		/// </summary>
		class GetTextBleRequestMethod : public BleRequestMethod {

		public:

			/// <summary>
			/// 
			/// </summary>
			GetTextBleRequestMethod(MeteoBluetoothMessage* gMessage, MeteoCommand rCommand);

			virtual int PerformAndWait(BleRequest* thisRequest);

			virtual BleRequestMethodType GetMethodType();

			string GetReturnText();

			json GetReturnJson();

		protected:

			MeteoBluetoothMessage* getMessage;

			MeteoCommand returnCommand = MeteoCommand::None;

			string returnText = "";

			ActionList<int, json> onReturn;

		};
	};

}}}



#endif