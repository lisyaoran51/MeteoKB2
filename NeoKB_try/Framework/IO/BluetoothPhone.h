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
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class BluetoothPhone : public Peripheral {


	public:

		virtual int SetDevice(Device* device);

		virtual int TriggerOnInput();

		/// <summary>
		/// 看有沒有沒丟完的file segment message
		/// </summary>
		bool CheckFileSegmentMessageOutputClear();

		/// <summary>
		/// 一般json con non message的時候用這個
		/// </summary>
		template<class _Type>
		int AddOnMessage(_Type* callableObject, function<int(InputState*)> callback, string name = "HandleCommand") {

			LOG(LogLevel::Fine) << "BluetoothPhone::AddOnMessage() : register handler into list.";

			OnMessage.Add(callableObject, callback, name);
			return 0;
		}

		/// <summary>
		/// 所有message都用這個，例如file、json、con、ack、non、ret
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
		/// 一般json con non message的時候用這個
		/// </summary>
		ActionList<int, InputState*> OnMessage;

		/// <summary>
		/// 所有message都用這個，例如file、json、con、ack、non、ret
		/// </summary>
		ActionList<int, InputState*> OnRawMessage;


	};

}}





#endif