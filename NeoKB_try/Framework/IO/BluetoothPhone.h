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

		template<class _Type>
		int AddOnCommand(_Type* callableObject, function<int(InputState*)> callback, string name = "HandleCommand") {

			LOG(LogLevel::Fine) << "BluetoothPhone::AddOnCommand() : register handler into list.";

			OnCommand.Add(callableObject, callback, name);
			return 0;
		}

		/// <summary>
		/// 寫入sm檔時的callback
		/// </summary>
		int AddOnStartWritingSmFile(MtoObject* callableObject, function<int(string)> callback, string name);
		int AddOnWriteSmFileSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

		/// <summary>
		/// 寫入新音色時的callback
		/// </summary>
		int AddOnStartWritingSoundFilePackage(MtoObject* callableObject, function<int(string)> callback, string name);
		int AddOnWriteSoundFilePackageSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

		/// <summary>
		/// 寫入新主程式時的callback
		/// </summary>
		int AddOnStartWritingProgram(MtoObject* callableObject, function<int(string)> callback, string name);
		int AddOnWriteProgramSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

	protected:

		BluetoothDevice* matchedBluetoothDevice;

	private:

		ActionList<int, InputState*> OnCommand;


	};

}}





#endif