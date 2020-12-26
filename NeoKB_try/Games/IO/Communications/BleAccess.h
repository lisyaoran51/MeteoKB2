#ifndef BLE_ACCESS_H
#define BLE_ACCESS_H


#include "../../../Framework/IO/Communications/CommunicationComponent.h"
#include "BleRequest.h"


using namespace Framework::IO::Communications;



namespace Games {
namespace IO{
namespace Communications{

	/// <summary>
	/// 這個不擺在framework，是因為他會有一些跟app綁死的資訊，例如user、token、加解密等等
	/// 除此之外，如果可以把這些跟app綁死的資訊分隔開，這個class就可以百再framework，有需要的話再來refactor
	/// </summary>
	class BleAccess : public TCommunicationComponent<BleRequest> {

	public:

		BleAccess(GameHost* gHost);

	protected:

		BluetoothPhone* bluetoothPhone = nullptr;

		virtual int run();

		/// <summary>
		/// 這邊要用strategy處理wifi和ble同時處理的問題
		/// </summary>
		virtual int handleRequest(CommunicationRequest* communicationRequest);
		

	};

}}}



#endif