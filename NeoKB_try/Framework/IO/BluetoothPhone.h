#ifndef BLUETOOTH_PHONE_H
#define BLUETOOTH_PHONE_H



#include <vector>
#include "Peripheral.h"
#include "../../Util/DataStructure/ActionList.h"
#include "../Input/InputState.h"
#include "../../Util/Log.h"



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


namespace Framework {
namespace IO {

	/// <summary>
	/// �����J���a��A�ѥX���a��
	/// </summary>
	class BluetoothPhone : public Peripheral {


	public:

		virtual int SetDevice(Device* device);

		virtual int TriggerOnInput();

		
		int AddOnCommand(MtoObject* callableObject, function<int(InputState*)> callback, string name = "HandleCommand") {

			LOG(LogLevel::Fine) << "BluetoothPhone::AddOnCommand() : register handler into list.";

			OnCommand.Add(callableObject, callback, name);
			return 0;
		}

		/// <summary>
		/// �g�Jsm�ɮɪ�callback
		/// </summary>
		template<class _Type>
		int AddOnStartWritingSmFile(_Type* callableObject, function<int(string)> callback, string name);

		template<class _Type>
		int AddOnWriteSmFileSuccess(_Type* callableObject, function<int(string)> callback, string name);

		/// <summary>
		/// �g�J�s����ɪ�callback
		/// </summary>
		template<class _Type>
		int AddOnStartWritingSoundFilePackage(_Type* callableObject, function<int(string)> callback, string name);
		template<class _Type>
		int AddOnWriteSoundFilePackageSuccess(_Type* callableObject, function<int(string)> callback, string name);

		/// <summary>
		/// �g�J�s�D�{���ɪ�callback
		/// </summary>
		template<class _Type>
		int AddOnStartWritingProgram(_Type* callableObject, function<int(string)> callback, string name);
		template<class _Type>
		int AddOnWriteProgramSuccess(_Type* callableObject, function<int(string)> callback, string name);

	protected:

		BluetoothDevice* matchedBluetoothDevice;

	private:

		ActionList<int, InputState*> OnCommand;


	};

}}





#endif