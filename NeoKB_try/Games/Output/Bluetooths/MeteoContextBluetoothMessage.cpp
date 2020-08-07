#include "MeteoContextBluetoothMessage.h"


using namespace Games::Output::Bluetooths;


MeteoContextBluetoothMessage::MeteoContextBluetoothMessage(MeteoCommand mCommand): MeteoBluetoothMessage(mCommand)
{
}

json & MeteoContextBluetoothMessage::GetContext()
{
	return context;
}
