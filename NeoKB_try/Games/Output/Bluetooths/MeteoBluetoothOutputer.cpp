#include "MeteoBluetoothOutputer.h"


using namespace Games::Output::Bluetooths;


int MeteoBluetoothOutputer::pushMessage(MeteoBluetoothMessage * outputMessage)
{
	if(bluetoothPhone)
		bluetoothPhone->PushOutputMessage(outputMessage);
	else
		LOG(LogLevel::Error) << "MeteoBluetoothOutputer::pushMessage : no bluetooth phone device registered.";
	
	return 0;
}
