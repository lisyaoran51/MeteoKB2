#ifndef METEO_KEYBOARD_DEVICE_H
#define METEO_KEYBOARD_DEVICE_H


#include "../../Framework/Devices/KeyboardDevice.h"
#include "MeteoPanelBoardV1.h"


using namespace Framework::Devices;



namespace Desktop {
namespace Devices {

	class MeteoKeyboardDevice : public KeyboardDevice {

		MeteoPanelBoardV1 * meteoPanelBoard = nullptr;

		int _debugCount = 0;

	public:

		MeteoKeyboardDevice(MeteoPanelBoardV1* panelBoard);

	protected:

		virtual int readFromDevice();

	};

}}





#endif