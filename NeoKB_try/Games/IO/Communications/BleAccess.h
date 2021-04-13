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


		/// <summary>
		/// 將buffer中的raw command更新進command中
		/// </summary>
		virtual int Update();

		Peripheral* GetPeripheral();

		BluetoothPhone* GetBluetoothPhone();

		int RegisterBleRequest(BleRequest* bleRequest);

		int UnregisterBleRequest(BleRequest* bleRequest);

		/// <summary>
		/// 刪掉時要怎麼保持thread safe?
		/// 用複製的是最安全的方法，但是效率不知道會慢多少
		/// 複製完還要clear
		/// </summary>
		deque<BluetoothMessage*>* GetInputRawCommand();

		mutex* GetRawCommandMutex();

		/// <summary>
		/// 拿到目前設定的mtu，這個還要確認norify是怎麼運作才知道能不能這樣寫
		/// </summary>
		int GetMtu();

		/* --------------CommunicationComponent.h-------------- */

		virtual int HandleState(InputState* inputEvent);

	protected:

		BluetoothPhone* bluetoothPhone = nullptr;

		vector<BleRequest*> bleRequests;

		mutable mutex bleRequestMutex;

		deque<BluetoothMessage*> inputRawCommand;

		/// <summary>
		/// 有時候handle on new command時剛剛好input raw command被別的request給lock起來，就要先把input command給丟進這個buffer裡面，
		/// 之後再找時間把buffer內容丟回inputRawCommand
		/// </summary>
		deque<BluetoothMessage*> inputRawCommandBuffer;

		mutable mutex rawCommandMutex;

		mutable mutex rawCommandBufferMutex;

		virtual int run();

		/// <summary>
		/// 這邊要用strategy處理wifi和ble同時處理的問題
		/// </summary>
		virtual int handleRequest(CommunicationRequest* communicationRequest);

		/// <summary>
		/// 把bluetooth phone新收到的所有訊息都clone一份進來
		/// 超過一定時間沒有input的時候就會清空，另外raw command也會保持一定數量，超出數量就會刪掉
		/// 刪掉的時候要怎麼保持thread safe?
		/// </summary>
		int handleOnRawMessage(InputState* inputState);
		

	};

}}}



#endif