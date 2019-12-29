#ifndef METEO_BLUETOOTH_DEVICE_H
#define METEO_BLUETOOTH_DEVICE_H

#include "../../Framework/Devices/BluetoothDevice.h"

using namespace Framework::Devices;


namespace Desktop {
namespace Devices {

	class MeteoBluetoothDevice : public BluetoothDevice {


	public:

	protected:

		virtual int readFromDevice();

		virtual int passToDevice();

	};

}}





#endif