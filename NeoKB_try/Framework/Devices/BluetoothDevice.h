#ifndef BLUETOOTH_DEVICE_H
#define BLUETOOTH_DEVICE_H

#include "InputDevice.h"
#include "OutputDevice.h"
#include "../IO/BluetoothPhone.h"


using namespace Framework::IO;



namespace Framework {
namespace Devices{


	class BluetoothDevice : public InputDevice, public OutputDevice {


	public:

		/// <summary>
		/// �ݦ��S���S�᧹��file segment message
		/// </summary>
		virtual bool CheckFileSegmentMessageOutputClear() = 0;

		/// <summary>
		/// �g�Jsm�ɮɪ�callback�A�令�\�brequest��
		/// </summary>
		//virtual int AddOnStartWritingSmFile(MtoObject* callableObject, function<int(string)> callback, string name) = 0;
		//virtual int AddOnWriteSmFileSuccess(MtoObject* callableObject, function<int(string)> callback, string name) = 0;

		/// <summary>
		/// �g�J�s����ɪ�callback�A�令�\�brequest��
		/// </summary>
		//virtual int AddOnStartWritingSoundFilePackage(MtoObject* callableObject, function<int(string)> callback, string name) = 0;
		//virtual int AddOnWriteSoundFilePackageSuccess(MtoObject* callableObject, function<int(string)> callback, string name) = 0;

		/// <summary>
		/// �g�J�s�D�{���ɪ�callback�A�令�\�brequest��
		/// </summary>
		//virtual int AddOnStartWritingProgram(MtoObject* callableObject, function<int(string)> callback, string name) = 0;
		//virtual int AddOnWriteProgramSuccess(MtoObject* callableObject, function<int(string)> callback, string name) = 0;

	protected:

		BluetoothPhone* matchedBluetoothPhone;

		virtual bool match(Peripheral* peripheral);

		virtual int handlePeripheralRegister(Peripheral* peripheral);

	};

}
}



#endif