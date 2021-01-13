#include "BluetoothCommand.h"

#include "../../../Util/Log.h"

using namespace Framework::Input::Commands;
using namespace Util;


BluetoothCommand::BluetoothCommand()
{
}

BluetoothCommand::BluetoothCommand(BluetoothCommand * copied)
{
	//TODO: �ƻs���e...
}

string BluetoothCommand::GetCommandName()
{
	return commandName;
}

bool BluetoothCommand::EqualTo(BluetoothCommand * other)
{
	// TODO: ��鷺��
	if (commandName == other->GetCommandName())
		return true;
	return false;
}

json & BluetoothCommand::GetContext()
{
	return context;
}

BluetoothCommand * BluetoothCommand::Clone()
{
	LOG(LogLevel::Depricated) << "BluetoothCommand::Clone(): create new command.";
	BluetoothCommand* btCommand = new BluetoothCommand();
	btCommand->commandName = commandName;
	LOG(LogLevel::Depricated) << "BluetoothCommand::Clone(): cloning context.";
	btCommand->context = context;

	return btCommand;
}
