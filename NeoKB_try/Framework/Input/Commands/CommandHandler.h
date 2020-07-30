#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "BluetoothCommand.h"

using json = nlohmann::json;

namespace Framework {
namespace Input {
namespace Commands{

	template<typename T>
	class CommandHandler {

	public:

		virtual int OnCommand(T* command) = 0;

	protected:
	};


}}}




#endif