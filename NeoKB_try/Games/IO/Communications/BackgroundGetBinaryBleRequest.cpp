#include "BackgroundGetBinaryBleRequest.h"

#include "BackgroundBleAccess.h"


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

int BackgroundGetBinaryBleRequest::Perform(CommunicationComponent * cComponent)
{

	LOG(LogLevel::Fine) << "int BackgroundGetBinaryBleRequest::Perform() : [" << GetTypeName() << "] start perform.";

	communicationComponent = cComponent;

	BackgroundBleAccess* bleAccess = dynamic_cast<BackgroundBleAccess*>(communicationComponent);
	// 讓ble access把raw message丟進來，好讓我們檢查有沒有ack或return
	bleAccess->RegisterBleRequest(this);

	requestStartTime = system_clock::now();
	exitRequested = false;

	// preform 丟資訊出去然後等回覆

	try {
		requestMethod->PerformAndWait(this);
	}
	catch (exception& e) {
		// 執行完畢以後就不讓ble access把raw message丟進來
		bleAccess->UnregisterBleRequest(this);
		throw e;
	}

	// 執行完畢以後就不讓ble access把raw message丟進來
	bleAccess->UnregisterBleRequest(this);

	// 查看成果有沒有錯，有錯的話就throw exception
	checkAndProcessFailure();

	return 0;
}

BackgroundGetBinaryBleRequest::BackgroundGetBinaryBleRequestMethod::BackgroundGetBinaryBleRequestMethod(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand aGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
	: GetBinaryBleRequest::GetBinaryBleRequestMethod::GetBinaryBleRequestMethod(fPath, gMessage, aGetCommand, tCommand, aTransferCommand, fCommand, rRetransferCommand, aFinishCommand)
{
}
