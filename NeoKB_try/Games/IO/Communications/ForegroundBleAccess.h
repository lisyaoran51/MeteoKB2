#ifndef FOREGROUND_BLE_ACCESS_H
#define FOREGROUND_BLE_ACCESS_H


#include "TBleAccess.h"




namespace Games {
namespace IO{
namespace Communications{

	/// <summary>
	/// 這個不擺在framework，是因為他會有一些跟app綁死的資訊，例如user、token、加解密等等
	/// 除此之外，如果可以把這些跟app綁死的資訊分隔開，這個class就可以百再framework，有需要的話再來refactor
	/// </summary>
	class ForegroundBleAccess : public TBleAccess<BleRequest> {

	public:

		ForegroundBleAccess(Host* gHost);

		virtual int Queue(CommunicationRequest* communicationRequest);

		virtual int Flush();

	protected:

	};

}}}



#endif