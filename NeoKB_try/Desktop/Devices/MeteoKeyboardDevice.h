#ifndef METEO_KEYBOARD_DEVICE_H
#define METEO_KEYBOARD_DEVICE_H


#include "../../Framework/Devices/KeyboardDevice.h"

using namespace Framework::Devices;



namespace Desktop {
namespace Devices {

	class MeteoKeyboardDevice : public KeyboardDevice {


	protected:

		virtual int readFromDevice();

		virtual int passToPeripheral(Peripheral* peripheral);

	};

}}





#endif