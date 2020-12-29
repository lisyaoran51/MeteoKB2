#ifndef BLE_ACCESS_H
#define BLE_ACCESS_H


#include "../../../Framework/IO/Communications/CommunicationComponent.h"
#include "BleRequest.h"
#include "../../../Framework/IO/BluetoothPhone.h"


using namespace Framework::IO::Communications;
using namespace Framework::IO;


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

		Peripheral* GetPeripheral();

		deque<BluetoothCommand*>& GetInputRawCommand();

		/// <summary>
		/// 拿到目前設定的mtu，這個還要確認norify是怎麼運作才知道能不能這樣寫
		/// </summary>
		int GetMtu();

	protected:

		BluetoothPhone* bluetoothPhone = nullptr;

		deque<BluetoothCommand*> inputRawCommand;

		virtual int run();

		/// <summary>
		/// 這邊要用strategy處理wifi和ble同時處理的問題
		/// </summary>
		virtual int handleRequest(CommunicationRequest* communicationRequest);

		/// <summary>
		/// 把bluetooth phone新收到的所有訊息都clone一份進來
		/// </summary>
		int handleOnRawCommand(InputState* inputState);
		

	};

}}}



#endif