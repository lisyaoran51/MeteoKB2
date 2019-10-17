#ifndef PANEL_DEVICE_H
#define PANEL_DEVICE_H

#include "InputDevice.h"
#include "OutputDevice.h"
#include "../IO/Panel.h"


using namespace Framework::IO;


namespace Framework {
namespace Devices{

	class PanelDevice : public InputDevice, public OutputDevice {

	public:

	protected:

		Panel* matchedPanel;

		virtual bool match(Peripheral* peripheral);

		virtual int handlePeripheralRegister(Peripheral* peripheral);




	};

}}



#endif