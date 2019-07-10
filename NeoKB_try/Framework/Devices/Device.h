#ifndef DEVICE_H
#define DEVICE_H

#include "../IO/MainInterface.h"

using namespace Framework::IO;

namespace Framework {
namespace Devices {

	class Device {

	public:

		virtual int Register(MainInterface* mainInterface) = 0;


	};

}}




#endif