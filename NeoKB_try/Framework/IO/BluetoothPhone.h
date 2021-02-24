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

		template<class _Type>
		int AddOnMessage(_Type* callableObject, function<int(InputState*)> callback, string name = "HandleCommand") {

			LOG(LogLevel::Fine) << "BluetoothPhone::AddOnMessage() : register handler into list.";

			OnMessage.Add(callableObject, callback, name);
			return 0;
		}

		template<class _Type>
		int AddOnRawCommand(_Type* callableObject, function<int(InputState*)> callback, string name = "HandleRawCommand") {

			LOG(LogLevel::Fine) << "BluetoothPhone::AddOnRawCommand() : register ble access into list.";

			OnRawCommand.Add(callableObject, callback, name);
			return 0;
		}

		/// <summary>
		/// 寫入sm檔時的callback
		/// </summary>
		//int AddOnStartWritingSmFile(MtoObject* callableObject, function<int(string)> callback, string name);
		//int AddOnWriteSmFileSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

		/// <summary>
		/// 寫入新音色時的callback
		/// </summary>
		//int AddOnStartWritingSoundFilePackage(MtoObject* callableObject, function<int(string)> callback, string name);
		//int AddOnWriteSoundFilePackageSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

		/// <summary>
		/// 寫入新主程式時的callback
		/// </summary>
		//int AddOnStartWritingProgram(MtoObject* callableObject, function<int(string)> callback, string name);
		//int AddOnWriteProgramSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

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
		ActionList<int, InputState*> OnRawCommand;


	};

}}





#endif