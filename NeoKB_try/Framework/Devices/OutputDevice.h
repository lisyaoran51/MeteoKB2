#ifndef OUTPUT_DEVICE_H
#define OUTPUT_DEVICE_H

#include "Device.h"

namespace Framework {
namespace Devices {

	class OutputDevice : virtual public Device {

	public:

		virtual int ProcessOutput() = 0;

	};

}}




#endif