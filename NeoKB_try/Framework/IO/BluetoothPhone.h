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
		/// �g�Jsm�ɮɪ�callback
		/// </summary>
		//int AddOnStartWritingSmFile(MtoObject* callableObject, function<int(string)> callback, string name);
		//int AddOnWriteSmFileSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

		/// <summary>
		/// �g�J�s����ɪ�callback
		/// </summary>
		//int AddOnStartWritingSoundFilePackage(MtoObject* callableObject, function<int(string)> callback, string name);
		//int AddOnWriteSoundFilePackageSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

		/// <summary>
		/// �g�J�s�D�{���ɪ�callback
		/// </summary>
		//int AddOnStartWritingProgram(MtoObject* callableObject, function<int(string)> callback, string name);
		//int AddOnWriteProgramSuccess(MtoObject* callableObject, function<int(string)> callback, string name);

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
		ActionList<int, InputState*> OnRawCommand;


	};

}}





#endif