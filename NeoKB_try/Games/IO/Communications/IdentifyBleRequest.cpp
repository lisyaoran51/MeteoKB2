#include "IdentifyBleRequest.h"

#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"


using namespace Games::IO::Communications;
using namespace Games::Output::Bluetooths;


IdentifyBleRequest::IdentifyBleRequest(string iMessage) : RegisterType("IdentifyBleRequest")
{
	MeteoContextBluetoothMessage* identifyMessage = new MeteoContextBluetoothMessage(MeteoCommand::SendEncryptedIdentifyMessage);

	json context;
	context["value"] = iMessage;

	identifyMessage->SetContextInJson(context);
	identifyMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

	requestMethod = new GetTextBleRequestMethod(identifyMessage, MeteoCommand::ReturnDecryptedIdentifyMessage);

	dynamic_cast<GetTextBleRequestMethod*>(requestMethod)->AddOnReturn(this, [=](json j) {



		return 0;
	}, "CheckIdentify");
	

}
