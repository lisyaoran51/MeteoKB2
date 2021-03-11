#ifndef FAULT_IDENTITY_BLE_REQUEST_H
#define FAULT_IDENTITY_BLE_REQUEST_H



#include "PostTextBleRequest.h"

namespace Games {
namespace IO{
namespace Communications{

	class FaultIdentityBleRequest : public PostTextBleRequest {

	public:

		/// <summary>
		/// �i����設�����~�õ���
		/// </summary>
		FaultIdentityBleRequest(string m);

	protected:

		string message = "";

	};

}}}






#endif