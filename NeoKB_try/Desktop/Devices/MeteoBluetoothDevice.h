#ifndef METEO_BLUETOOTH_DEVICE_H
#define METEO_BLUETOOTH_DEVICE_H

#include "../../Framework/Devices/BluetoothDevice.h"
#include "MeteoBluetoothPhoneV1.h"

using namespace Framework::Devices;


namespace Desktop {
namespace Devices {

	class MeteoBluetoothDevice : public BluetoothDevice {

		MeteoBluetoothPhoneV1* meteoBluetoothPhone = nullptr;

	public:

		MeteoBluetoothDevice(MeteoBluetoothPhoneV1* mBluetoothPhone);

	protected:

		virtual int readFromDevice();

		virtual int passToDevice();

	};

}}





#endif