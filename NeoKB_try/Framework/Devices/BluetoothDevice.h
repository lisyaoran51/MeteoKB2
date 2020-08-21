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
		template<class _Type>
		int AddOnStartWritingSmFile(_Type* callableObject, function<int(string)> callback, string name) {
			return onStartWritingSmFile.Add(callableObject, callback, name);
		}
		template<class _Type>
		int AddOnWriteSmFileSuccess(_Type* callableObject, function<int(string)> callback, string name) {
			return onWriteSmFileSuccess.Add(callableObject, callback, name);
		}

		/// <summary>
		/// 寫入新音色時的callback
		/// </summary>
		template<class _Type>
		int AddOnStartWritingSoundFilePackage(_Type* callableObject, function<int(string)> callback, string name) {
			return onStartWritingSoundFilePackage.Add(callableObject, callback, name);
		}
		template<class _Type>
		int AddOnWriteSoundFilePackageSuccess(_Type* callableObject, function<int(string)> callback, string name) {
			return onWriteSoundFilePackageSuccess.Add(callableObject, callback, name);
		}

		/// <summary>
		/// 寫入新主程式時的callback
		/// </summary>
		template<class _Type>
		int AddOnStartWritingProgram(_Type* callableObject, function<int(string)> callback, string name) {
			return onStartWritingProgram.Add(callableObject, callback, name);
		}
		template<class _Type>
		int AddOnWriteProgramSuccess(_Type* callableObject, function<int(string)> callback, string name) {
			return onWriteProgramSuccess.Add(callableObject, callback, name);
		}

	protected:

		ActionList<int, string> onStartWritingSmFile;
		ActionList<int, string> onWriteSmFileSuccess;

		ActionList<int, string> onStartWritingSoundFilePackage;
		ActionList<int, string> onWriteSoundFilePackageSuccess;

		ActionList<int, string> onStartWritingProgram;
		ActionList<int, string> onWriteProgramSuccess;

		BluetoothPhone* matchedBluetoothPhone;

		virtual bool match(Peripheral* peripheral);

		virtual int handlePeripheralRegister(Peripheral* peripheral);

	};

}
}



#endif