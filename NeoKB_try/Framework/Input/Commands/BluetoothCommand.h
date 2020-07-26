#ifndef BLUETOOTH_COMMAND_H
#define BLUETOOTH_COMMAND_H

#include "../../../ThirdParty/json/json.hpp"

using json = nlohmann::json;

namespace Framework {
namespace Input {
namespace Commands{

	class BluetoothCommand {

	public:

		BluetoothCommand(BluetoothCommand* copied);

		string GetCommandName();

	protected:

		string commandName;
	};

	template<typename T>
	class TBluetoothCommand : public BluetoothCommand {

	public:

		TBluetoothCommand(T c) {
			command = c;
		}

	protected:

		T command;



	};

}}}




#endif