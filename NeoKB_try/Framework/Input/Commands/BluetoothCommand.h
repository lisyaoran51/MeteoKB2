#ifndef BLUETOOTH_COMMAND_H
#define BLUETOOTH_COMMAND_H

#include "../../../ThirdParty/json/json.hpp"

using json = nlohmann::json;

namespace Framework {
namespace Input {
namespace Commands{

	class BluetoothCommand {

	public:

		BluetoothCommand();

		BluetoothCommand(BluetoothCommand* copied);

		string GetCommandName();

		virtual bool EqualTo(BluetoothCommand* other);

		json& GetContext();

	protected:

		string commandName;

		json context;

	};

	template<typename T>
	class TBluetoothCommand : public BluetoothCommand {

	public:

		TBluetoothCommand(T c, json text) {
			command = c;
			context = text;
		}

		TBluetoothCommand(T c) {
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