#include "BluetoothCommand.h"


using namespace Framework::Input::Commands;


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
	BluetoothCommand* btCommand = new BluetoothCommand();
	btCommand->commandName = commandName;
	btCommand->context = context;

	return btCommand;
}
