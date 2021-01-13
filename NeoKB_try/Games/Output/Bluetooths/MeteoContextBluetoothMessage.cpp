#include "MeteoContextBluetoothMessage.h"

#include "../../../Util/Log.h"


using namespace Games::Output::Bluetooths;
using namespace Util;
using namespace std;


MeteoContextBluetoothMessage::MeteoContextBluetoothMessage(MeteoCommand mCommand): MeteoBluetoothMessage(mCommand)
{
}

int MeteoContextBluetoothMessage::SetAccessType(MeteoBluetoothMessageAccessType aType)
{
	return 0;
}

int MeteoContextBluetoothMessage::SetContext(char * c, int cSize)
{
	

	if (accessType == MeteoBluetoothMessageAccessType::WriteOnly ||
		accessType == MeteoBluetoothMessageAccessType::ReadWrite) {

		if (contextSize != 0) {
			delete[] context;
			contextSize = 0;
		}

		context = new char[cSize];
		memcpy(context, c, cSize);
		contextSize = cSize;

	}
	else {
		LOG(LogLevel::Error) << "MeteoContextBluetoothMessage::SetContext() : unavailable to write.";
		return -1;
	}


	return 0;
}

int MeteoContextBluetoothMessage::SetContextInJson(json cInJson)
{
	if (accessType == MeteoBluetoothMessageAccessType::ReadWrite ||
		accessType == MeteoBluetoothMessageAccessType::WriteOnly) {
		contextInJson = cInJson;
	}
}

string MeteoContextBluetoothMessage::GetContext()
{
	if (accessType == MeteoBluetoothMessageAccessType::ReadOnly ||
		accessType == MeteoBluetoothMessageAccessType::ReadWrite) {
		return string(context);
	}

	LOG(LogLevel::Error) << "MeteoContextBluetoothMessage::GetContext() : unavailable to read.";
	return "";
}

json MeteoContextBluetoothMessage::GetContextInJson()
{
	if (accessType == MeteoBluetoothMessageAccessType::ReadOnly) {
		return contextInJson;

	}

	LOG(LogLevel::Error) << "MeteoContextBluetoothMessage::GetContextInJson() : unavailable to get json context.";
	return json();
}
