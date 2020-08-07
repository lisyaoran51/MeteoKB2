#include "MeteoBluetoothMessage.h"


using namespace Games::Output::Bluetooths;


MeteoBluetoothMessage::MeteoBluetoothMessage(MeteoCommand mCommand)
{
}

MeteoCommand MeteoBluetoothMessage::GetCommand()
{
	return meteoCommand;
}

string MeteoBluetoothMessage::ToString()
{
	// 好像沒用，要做table太麻煩
	return string();
}
