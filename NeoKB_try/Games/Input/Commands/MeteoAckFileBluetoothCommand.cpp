#include "MeteoAckFileBluetoothCommand.h"

using namespace Games::Input::Commands;


MeteoAckFileBluetoothCommand::MeteoAckFileBluetoothCommand(MeteoCommand c) : MeteoBluetoothCommand(c)
{
}
 
MeteoAckFileBluetoothCommand::MeteoAckFileBluetoothCommand(MeteoCommand c, int o, string fName) : MeteoBluetoothCommand(c)
{

}

int MeteoAckFileBluetoothCommand::SetOrder(int o)
{
	order = o;
	return 0;
}

int MeteoAckFileBluetoothCommand::GetOrder()
{
	return order;
}

int MeteoAckFileBluetoothCommand::SetFileName(string fName)
{
	fileName = fName;
	return 0;
}

string MeteoAckFileBluetoothCommand::GetFileName()
{
	return fileName;
}
