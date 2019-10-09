#ifndef DEVICE_H
#define DEVICE_H

#include "../IO/MainInterface.h"

using namespace Framework::IO;

namespace Framework {
namespace Devices {


	class Device {

		string peripheralType;

	public:

		virtual int Register(MainInterface* mainInterface) = 0;

		virtual bool Match(Peripheral* peripheral) = 0;

		virtual int HandlePeripheralRegister(Peripheral* peripheral) = 0;

	protected:

		Peripheral* matchedPeripheral;

	};

}}




#endif