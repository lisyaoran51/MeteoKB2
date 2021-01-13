#include "MeteoBluetoothMessage.h"


using namespace Games::Output::Bluetooths;


MeteoBluetoothMessage::MeteoBluetoothMessage(MeteoCommand mCommand) : TBluetoothMessage(mCommand)
{
}

string MeteoBluetoothMessage::ToString()
{
	// 好像沒用，要做table太麻煩
	return string();
}

int MeteoBluetoothMessage::SetAccessType(MeteoBluetoothMessageAccessType aType)
{
	accessType = aType;
	return 0;
}
