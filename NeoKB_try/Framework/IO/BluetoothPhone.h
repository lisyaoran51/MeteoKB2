#ifndef BLUETOOTH_PHONE_H
#define BLUETOOTH_PHONE_H



#include <vector>
#include "Peripheral.h"
#include "../../Util/DataStructure/ActionList.h"
#include "../Input/InputState.h"
#include "../../Util/Log.h"
#include "../Allocation/Hierachal/MtoObject.h"



namespace Framework {
namespace Devices{
	class Device;
	class BluetoothDevice;
}}



using namespace std;
using namespace Util::DataStructure;
using namespace Framework::Input;
using namespace Framework::Devices;
using namespace Util;
using namespace Framework::Allocation::Hierachal;


namespace Framework {
namespace IO {

	/// <summary>
	/// �����J���a��A�ѥX���a��
	/// </summary>
	class BluetoothPhone : public Peripheral {


	public:

		virtual int SetDevice(Device* device);

		virtual int TriggerOnInput();

		/// <summary>
		/// �ݦ��S���S�᧹��file segment message
		/// </summary>
		bool CheckFileSegmentMessageOutputClear();

		/// <summary>
		/// �@��json con non message���ɭԥγo��
		/// </summary>
		template<class _Type>
		int AddOnMessage(_Type* callableObject, function<int(InputState*)> callback, string name = "HandleCommand") {

			LOG(LogLevel::Fine) << "BluetoothPhone::AddOnMessage() : register handler into list.";

			OnMessage.Add(callableObject, callback, name);
			return 0;
		}

		/// <summary>
		/// �Ҧ�message���γo�ӡA�Ҧpfile�Bjson�Bcon�Back�Bnon�Bret
		/// </summary>
		template<class _Type>
		int AddOnRawMessage(_Type* callableObject, function<int(InputState*)> callback, string name = "HandleRawMessage") {

			LOG(LogLevel::Fine) << "BluetoothPhone::AddOnRawMessage() : register ble access into list.";

			OnRawMessage.Add(callableObject, callback, name);
			return 0;
		}

	protected:

		BluetoothDevice* matchedBluetoothDevice;

	private:

		/// <summary>
		/// �@��json con non message���ɭԥγo��
		/// </summary>
		ActionList<int, InputState*> OnMessage;

		/// <summary>
		/// �Ҧ�message���γo�ӡA�Ҧpfile�Bjson�Bcon�Back�Bnon�Bret
		/// </summary>
		ActionList<int, InputState*> OnRawMessage;


	};

}}





#endif