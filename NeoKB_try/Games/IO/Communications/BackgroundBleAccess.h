#ifndef BACKGROUND_BLE_ACCESS_H
#define BACKGROUND_BLE_ACCESS_H


#include "BackgroundGetBinaryBleRequest.h"
#include "TBleAccess.h"


namespace Games {
namespace IO{
namespace Communications{

	/// <summary>
	/// ¦b­I´º¶]ªºaccess
	/// </summary>
	class BackgroundBleAccess : public TBleAccess<BackgroundGetBinaryBleRequest> {

	public:

		BackgroundBleAccess(Host* gHost);

		virtual int Queue(CommunicationRequest* communicationRequest);

		virtual int Flush();


	};

}}}



#endif