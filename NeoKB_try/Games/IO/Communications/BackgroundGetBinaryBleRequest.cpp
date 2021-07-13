#include "BackgroundGetBinaryBleRequest.h"


using namespace Games::IO::Communications;


BackgroundGetBinaryBleRequest::BackgroundGetBinaryBleRequest(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand aGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
	: GetBinaryBleRequest(fPath, gMessage, aGetCommand, tCommand, aTransferCommand, fCommand, rRetransferCommand, aFinishCommand) , RegisterType("BackgroundGetBinaryBleRequest")
{
	BackgroundGetBinaryBleRequestMethod* method = new BackgroundGetBinaryBleRequestMethod(fPath,
		gMessage,
		aGetCommand,
		tCommand,
		aTransferCommand,
		fCommand,
		rRetransferCommand,
		aFinishCommand);

	requestMethod = method;

	// 因為在background，所以timeout設長一點
	timeout = 10;
}

int BackgroundGetBinaryBleRequest::ChooseCommunicationComponentToPerform()
{
	LOG(LogLevel::Depricated) << "int BackgroundGetBinaryBleRequest::ChooseCommunicationComponentToPerform() : [" << GetTypeName() << "] request finding component.";

	map<string, deque<CommunicationRequest*>*>::iterator it;

	for (it = acceptedCommunicationComponentRequestQueues.begin(); it != acceptedCommunicationComponentRequestQueues.end(); ++it) {
		if (it->first == "BackgroundBleAccess") {

			LOG(LogLevel::Debug) << "int BackgroundGetBinaryBleRequest::ChooseCommunicationComponentToPerform() : [" << GetTypeName() << "] component found.";
			it->second->push_back(this);
		}
	}
	return 0;
}

BackgroundGetBinaryBleRequest::BackgroundGetBinaryBleRequestMethod::BackgroundGetBinaryBleRequestMethod(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand aGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
	: GetBinaryBleRequest::GetBinaryBleRequestMethod::GetBinaryBleRequestMethod(fPath, gMessage, aGetCommand, tCommand, aTransferCommand, fCommand, rRetransferCommand, aFinishCommand)
{
}
