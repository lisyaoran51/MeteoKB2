#include "BackgroundGetBinaryBleRequest.h"


using namespace Games::IO::Communications;


BackgroundGetBinaryBleRequest::BackgroundGetBinaryBleRequest(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand aGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
	: GetBinaryBleRequest(fPath, gMessage, aGetCommand, tCommand, aTransferCommand, fCommand, rRetransferCommand, aFinishCommand) , RegisterType("BackgroundGetBinaryBleRequest")
{
}

int BackgroundGetBinaryBleRequest::ChooseCommunicationComponentToPerform()
{
	return 0;
}

BackgroundGetBinaryBleRequest::BackgroundGetBinaryBleRequestMethod::BackgroundGetBinaryBleRequestMethod(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand aGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
	: GetBinaryBleRequest::GetBinaryBleRequestMethod::GetBinaryBleRequestMethod(fPath, gMessage, aGetCommand, tCommand, aTransferCommand, fCommand, rRetransferCommand, aFinishCommand)
{
}
