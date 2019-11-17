#ifndef BLUETOOTH_PHONE_H
#define BLUETOOTH_PHONE_H



#include <vector>
#include "Peripheral.h"
#include "../../Util/DataStructure/ActionList.h"
#include "../Input/InputState.h"



namespace Framework {
namespace Devices{
	class Device;
	class BluetoothDevice;
}}



using namespace std;
using namespace Util::DataStructure;
using namespace Framework::Input;
using namespace Framework::Devices;



namespace Framework {
namespace IO {

	/// <summary>
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class BluetoothPhone : public Peripheral {


	public:

		virtual int SetDevice(Device* device);

		virtual int TriggerOnInput();

		template<class _Type>
		int AddOnCommand(_Type* callableObject, function<int(InputState*)> callback, string name = "HandleCommand") {

			LOG(LogLevel::Fine) << "BluetoothPhone::AddOnCommand() : register handler into list.";

			OnCommand.Add(callableObject, callback, name);
			return 0;
		}

	protected:

		BluetoothDevice* matchedBluetoothDevice;

	private:

		ActionList<int, InputState*> OnCommand;


	};


	

}
}









#endif