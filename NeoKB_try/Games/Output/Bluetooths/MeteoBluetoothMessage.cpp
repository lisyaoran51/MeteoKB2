#include "MeteoBluetoothMessage.h"


using namespace Games::Output::Bluetooths;


MeteoBluetoothMessage::MeteoBluetoothMessage(MeteoCommand mCommand) : TBluetoothMessage(mCommand)
{
}

string MeteoBluetoothMessage::ToString()
{
	// �n���S�ΡA�n��table�ӳ·�
	return string();
}

int MeteoBluetoothMessage::SetAccessType(MeteoBluetoothMessageAccessType aType)
{
	accessType = aType;
	return 0;
}
