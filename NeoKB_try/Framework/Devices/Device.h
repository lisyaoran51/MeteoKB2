#ifndef DEVICE_H
#define DEVICE_H



#include "../IO/Peripheral.h"
#include "../IO/MainInterface.h"



using namespace Framework::IO;



namespace Framework {
namespace Devices {


	class Device {

		string peripheralType;

	public:

		virtual int Register(MainInterface* mainInterface);

	protected:

		Peripheral* matchedPeripheral = nullptr;

		virtual bool match(Peripheral* peripheral) = 0;

		virtual int handlePeripheralRegister(Peripheral* peripheral) = 0;

	};

}}




#endif