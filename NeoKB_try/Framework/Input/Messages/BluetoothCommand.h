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
		/// �ƻs�@��context�X�h�A�^�ǭȬOsize
		/// </summary>
		int GetContext(char** c);

		/// <summary>
		/// �n���n�]�t\0 ?
		/// </summary>
		int GetContextSize();

	protected:

		string commandName;

		char* context = nullptr;

		/// <summary>
		/// �n���n�]�t\0 ?
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