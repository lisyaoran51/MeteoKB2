#include "MeteoBluetoothCommand.h"

#include "../../../Util/Log.h"

using namespace Games::Input::Commands;
using namespace Util;


MeteoBluetoothCommand::MeteoBluetoothCommand(MeteoCommand c, json text): TBluetoothCommand<MeteoCommand>(c, text)
{
}

MeteoBluetoothCommand::MeteoBluetoothCommand(MeteoCommand c): TBluetoothCommand<MeteoCommand>(c)
{
}

BluetoothCommand * MeteoBluetoothCommand::Clone()
{
	MeteoBluetoothCommand* btCommand = new MeteoBluetoothCommand(command);

	LOG(LogLevel::Finest) << "MeteoBluetoothCommand::Clone(): start get context.";
	btCommand->GetContext() = context;
	LOG(LogLevel::Finest) << "MeteoBluetoothCommand::Clone(): cloning context over.";
	return btCommand;
}
