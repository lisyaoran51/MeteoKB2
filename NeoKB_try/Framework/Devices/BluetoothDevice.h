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
		/// 寫入sm檔時的callback
		/// </summary>
		virtual int AddOnStartWritingSmFile(MtoObject* callableObject, function<int(string)> callback, string name) = 0;
		virtual int AddOnWriteSmFileSuccess(MtoObject* callableObject, function<int(string)> callback, string name) = 0;

		/// <summary>
		/// 寫入新音色時的callback
		/// </summary>
		virtual int AddOnStartWritingSoundFilePackage(MtoObject* callableObject, function<int(string)> callback, string name) = 0;
		virtual int AddOnWriteSoundFilePackageSuccess(MtoObject* callableObject, function<int(string)> callback, string name) = 0;

		/// <summary>
		/// 寫入新主程式時的callback
		/// </summary>
		virtual int AddOnStartWritingProgram(MtoObject* callableObject, function<int(string)> callback, string name) = 0;
		virtual int AddOnWriteProgramSuccess(MtoObject* callableObject, function<int(string)> callback, string name) = 0;

	protected:

		BluetoothPhone* matchedBluetoothPhone;

		virtual bool match(Peripheral* peripheral);

		virtual int handlePeripheralRegister(Peripheral* peripheral);

	};

}
}



#endif