#ifndef BLUETOOTH_COMMAND_H
#define BLUETOOTH_COMMAND_H

#include "../../../ThirdParty/json/json.hpp"

using namespace std;

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

		virtual BluetoothCommand* Clone();

		/// <summary>
		/// 複製一份context出去，回傳值是size
		/// </summary>
		int GetContext(char** c);

		/// <summary>
		/// 要不要包含\0 ?
		/// </summary>
		int GetContextSize();

	protected:

		string commandName;

		char* context = nullptr;

		/// <summary>
		/// 要不要包含\0 ?
		/// </summary>
		int contextSize = -1;

	};

	template<typename T>
	class TBluetoothCommand : public BluetoothCommand {

	public:

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