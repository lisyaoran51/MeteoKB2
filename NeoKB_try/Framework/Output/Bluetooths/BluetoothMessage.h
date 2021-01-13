#ifndef BLUETOOTH_MESSAGE_H
#define BLUETOOTH_MESSAGE_H

#include "../OutputMessage.h"


using namespace Framework::Output;


namespace Framework {
namespace Output{
namespace Bluetooths{

	class BluetoothMessage : public OutputMessage {

	public:

		virtual BluetoothMessage* Clone() = 0;

	protected:

	};

	template<typename T>
	class TBluetoothMessage : public BluetoothMessage {

	public:

		TBluetoothMessage(T c) {
			command = c;
		}

		T GetCommand() {
			return command;
		}

	protected:

		T command;

	};

}}}








#endif