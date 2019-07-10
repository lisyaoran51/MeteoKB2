#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include "Device.h"

namespace Framework {
namespace Devices {

	class InputDevice : virtual public Device {


	public:

		virtual int ScanInput() = 0;

	};

}}




#endif