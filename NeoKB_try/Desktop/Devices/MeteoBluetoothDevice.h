#ifndef METEO_BLUETOOTH_DEVICE_H
#define METEO_BLUETOOTH_DEVICE_H

#include "../../Framework/Devices/BluetoothDevice.h"

using namespace Framework::Devices;


namespace Desktop {
namespace Devices {

	class MeteoBluetoothDevice : public BluetoothDevice {


	public:

		/// <summary>
		/// 再input thread每一輪值形一次，不同device自己去定這一輪他要做什麼書出
		/// </summary>
		virtual int ProcessOutput();

	protected:

		virtual int readFromDevice();

		virtual int passToPeripheral(Peripheral* peripheral);

	};

}}





#endif