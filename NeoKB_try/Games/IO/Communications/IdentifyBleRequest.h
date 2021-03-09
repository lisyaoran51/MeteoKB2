#ifndef IDENTIFY_BLE_REQUEST_H
#define IDENTIFY_BLE_REQUEST_H

#include "GetTextBleRequest.h"

namespace Games {
namespace IO{
namespace Communications{

	class IdentifyBleRequest : public GetTextBleRequest {

	public:

		/// <summary>
		/// ��identify message��X�h�A�b����ݸѱK��A��^��
		/// </summary>
		IdentifyBleRequest(string iMessage);

	protected:

		string identifyCode = "";

		virtual int checkAndProcessFailure();

	};

}}}



#endif