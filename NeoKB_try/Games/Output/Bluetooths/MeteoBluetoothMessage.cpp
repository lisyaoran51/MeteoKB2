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
	// �n���S�ΡA�n��table�ӳ·�
	return string();
}
