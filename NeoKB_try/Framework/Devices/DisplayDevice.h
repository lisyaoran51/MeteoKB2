#ifndef DISPLAY_DEVICE_H
#define DISPLAY_DEVICE_H


#include "OutputDevice.h"
#include "../Graphic/Map/Map.h"
#include "../IO/Display.h"

using namespace Framework::Graphic::Maps;
using namespace Framework::IO;


namespace Framework {
namespace Devices {

	class DisplayDevice : public OutputDevice {

	public:

		virtual int Show(Map* m) = 0;

	protected:

		virtual bool match(Peripheral* peripheral);

		virtual int handlePeripheralRegister(Peripheral* peripheral);

		Display* matchedDisplay;
	};

}}



#endif