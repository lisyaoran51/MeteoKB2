#ifndef BLUETOOTH_DEVICE_H
#define BLUETOOTH_DEVICE_H

#include "InputDevice.h"
#include "OutputDevice.h"
#include "../IO/BluetoothPhone.h"


using namespace Framework::IO;



namespace Framework {
namespace Devices{


	class BluetoothDevice : public InputDevice, public OutputDevice {


	protected:

		BluetoothPhone* matchedBluetoothPhone;

		virtual bool match(Peripheral* peripheral);

		virtual int handlePeripheralRegister(Peripheral* peripheral);




	};

}
}



#endif