#ifndef METEO_KEYBOARD_DEVICE_H
#define METEO_KEYBOARD_DEVICE_H


#include "../../Framework/Devices/KeyboardDevice.h"
#include "MeteoMcuV1.h"


using namespace Framework::Devices;



namespace Desktop {
namespace Devices {

	class MeteoKeyboardDevice : public KeyboardDevice {

		MeteoMcuV1 * meteoMcu = nullptr;

		int _debugCount = 0;

	public:

		MeteoKeyboardDevice(MeteoMcuV1* mMcu);

	protected:

		virtual int readFromDevice();

	};

}}





#endif