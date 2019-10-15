#ifndef BLUETOOTH_DEVICE_H
#define BLUETOOTH_DEVICE_H

#include "Device.h"




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