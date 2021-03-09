#include "IdentifyBleRequest.h"

#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"


using namespace Games::IO::Communications;
using namespace Games::Output::Bluetooths;


IdentifyBleRequest::IdentifyBleRequest(string iMessage) : RegisterType("IdentifyBleRequest")
{
	identifyCode = iMessage; // ����n�令�ѽX�����r��

	MeteoContextBluetoothMessage* identifyMessage = new MeteoContextBluetoothMessage(MeteoCommand::SendEncryptedIdentifyMessage);

	json context;
	context["Value"] = iMessage;

	identifyMessage->SetContextInJson(context);
	identifyMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

	requestMethod = new GetTextBleRequestMethod(identifyMessage, MeteoCommand::ReturnDecryptedIdentifyMessage);

}

int IdentifyBleRequest::checkAndProcessFailure()
{
	if (dynamic_cast<GetTextBleRequestMethod*>(requestMethod)->GetReturnJson()["Value"].get<string>() != identifyCode) {
		throw exception("IdentifyBleRequest::checkAndProcessFailure(): identify failed");
	}

	return 0;
}
