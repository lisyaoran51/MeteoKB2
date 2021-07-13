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

	// �]���bbackground�A�ҥHtimeout�]���@�I
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
	// ��ble access��raw message��i�ӡA�n���ڭ��ˬd���S��ack��return
	bleAccess->RegisterBleRequest(this);

	requestStartTime = system_clock::now();
	exitRequested = false;

	// preform ���T�X�h�M�ᵥ�^��

	try {
		requestMethod->PerformAndWait(this);
	}
	catch (exception& e) {
		// ���槹���H��N����ble access��raw message��i��
		bleAccess->UnregisterBleRequest(this);
		throw e;
	}

	// ���槹���H��N����ble access��raw message��i��
	bleAccess->UnregisterBleRequest(this);

	// �d�ݦ��G���S�����A�������ܴNthrow exception
	checkAndProcessFailure();

	return 0;
}

BackgroundGetBinaryBleRequest::BackgroundGetBinaryBleRequestMethod::BackgroundGetBinaryBleRequestMethod(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand aGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
	: GetBinaryBleRequest::GetBinaryBleRequestMethod::GetBinaryBleRequestMethod(fPath, gMessage, aGetCommand, tCommand, aTransferCommand, fCommand, rRetransferCommand, aFinishCommand)
{
}
