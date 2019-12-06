#ifndef METEO_KEYBOARD_DEVICE_H
#define METEO_KEYBOARD_DEVICE_H


#include "../../Framework/Devices/KeyboardDevice.h"

using namespace Framework::Devices;



namespace Desktop {
namespace Devices {

	class MeteoKeyboardDevice : public KeyboardDevice {

		int _debugCount = 0;
	protected:

		virtual int readFromDevice();

	};

}}





#endif