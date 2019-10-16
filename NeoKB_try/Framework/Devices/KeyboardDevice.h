#ifndef KEYBOARD_DEVICE_H
#define KEYBOARD_DEVICE_H


#include "InputDevice.h"
#include "../IO/Keyboard.h"

using namespace Framework::IO;


namespace Framework {
namespace Devices {

	class KeyboardDevice : public InputDevice {



	public:

		

	protected:


		Keyboard* matchedKeyboard;

		virtual bool match(Peripheral* peripheral);

		virtual int handlePeripheralRegister(Peripheral* peripheral);


	};

}}



#endif