#include "IdentifyBleRequest.h"

#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"


using namespace Games::IO::Communications;
using namespace Games::Output::Bluetooths;


IdentifyBleRequest::IdentifyBleRequest(string iMessage) : RegisterType("IdentifyBleRequest")
{
	identifyCode = iMessage; // 之後要改成解碼完的字串

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
