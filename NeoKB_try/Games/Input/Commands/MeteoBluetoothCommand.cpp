#include "MeteoBluetoothCommand.h"


using namespace Games::Input::Commands;


MeteoBluetoothCommand::MeteoBluetoothCommand(MeteoCommand c, json text): TBluetoothCommand<MeteoCommand>(c, text)
{
}

MeteoBluetoothCommand::MeteoBluetoothCommand(MeteoCommand c): TBluetoothCommand<MeteoCommand>(c)
{
}

BluetoothCommand * MeteoBluetoothCommand::Clone()
{
	MeteoBluetoothCommand* btCommand = new MeteoBluetoothCommand(command, context);

	return btCommand;
}
