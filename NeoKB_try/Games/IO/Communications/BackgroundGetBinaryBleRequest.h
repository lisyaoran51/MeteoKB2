#ifndef BACKGROUND_GET_BINARY_BLE_REQUEST_H
#define BACKGROUND_GET_BINARY_BLE_REQUEST_H

#include "GetBinaryBleRequest.h"


using namespace Util::DataStructure;


namespace Games {
namespace IO{
namespace Communications{

	class BackgroundGetBinaryBleRequest : public GetBinaryBleRequest {

	public:

		BackgroundGetBinaryBleRequest(string fPath,
			MeteoBluetoothMessage* gMessage,
			MeteoCommand ackGetCommand,
			MeteoCommand tCommand,
			MeteoCommand aTransferCommand,
			MeteoCommand fCommand,
			MeteoCommand rRetransferCommand,
			MeteoCommand aFinishCommand);

		/// <summary>
		/// 有時一個request可以同時被wifi、ble、bt等多種communication component執行，這時request要先接收目前可選用的component有哪些，自己選定要用哪種component
		/// 然後再執行
		/// </summary>
		virtual int ChooseCommunicationComponentToPerform();

	protected:

		/// <summary>
		/// get binary request的執行動作
		/// </summary>
		class BackgroundGetBinaryBleRequestMethod : public GetBinaryBleRequestMethod {

		public:

			/// <summary>
			/// 資料夾位置最後面部要加上斜線
			/// </summary>
			BackgroundGetBinaryBleRequestMethod(string fPath,
				MeteoBluetoothMessage* gMessage,
				MeteoCommand aGetCommand,
				MeteoCommand tCommand,
				MeteoCommand aTransferCommand,
				MeteoCommand fCommand,
				MeteoCommand rRetransferCommand,
				MeteoCommand aFinishCommand);

		};

	};

}}}

#endif