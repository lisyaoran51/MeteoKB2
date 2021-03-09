#ifndef IDENTIFY_BLE_REQUEST_H
#define IDENTIFY_BLE_REQUEST_H

#include "GetTextBleRequest.h"

namespace Games {
namespace IO{
namespace Communications{

	class IdentifyBleRequest : public GetTextBleRequest {

	public:

		/// <summary>
		/// 把identify message丟出去，在手機端解密後再丟回來
		/// </summary>
		IdentifyBleRequest(string iMessage);

	protected:

		string identifyCode = "";

		virtual int checkAndProcessFailure();

	};

}}}



#endif