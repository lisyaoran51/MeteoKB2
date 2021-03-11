#include "FaultIdentityBleRequest.h"

#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"


using namespace Games::IO::Communications;
using namespace Games::Output::Bluetooths;


FaultIdentityBleRequest::FaultIdentityBleRequest(string m) : RegisterType("FaultIdentityBleRequest")
{
	message = m;

	MeteoContextBluetoothMessage* postMessage = new MeteoContextBluetoothMessage(MeteoCommand::FaultIdentity);

	json context;
	context["Message"] = m;

	postMessage->SetContextInJson(context);
	postMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

	PostTextBleRequestMethod* method = new PostTextBleRequestMethod(postMessage);

	requestMethod = method;

}
