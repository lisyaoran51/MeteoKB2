#include "MeteoContextBluetoothMessage.h"

#include "../../../Util/Log.h"


using namespace Games::Output::Bluetooths;
using namespace Util;
using namespace std;


MeteoContextBluetoothMessage::MeteoContextBluetoothMessage(MeteoCommand mCommand): MeteoBluetoothMessage(mCommand)
{
}

string MeteoContextBluetoothMessage::ToString()
{
	return GetContext();
}

BluetoothMessage * MeteoContextBluetoothMessage::Clone()
{
	MeteoContextBluetoothMessage* clonedMessage = new MeteoContextBluetoothMessage(command);

	if (accessType == MeteoBluetoothMessageAccessType::WriteOnly) {
		if (contextSize > 0)
			clonedMessage->SetContext(context, contextSize);

		if (contextInJson.size() > 0)
			clonedMessage->SetContextInJson(contextInJson);
	}
	else if (accessType == MeteoBluetoothMessageAccessType::ReadOnly) {
		clonedMessage->SetContextInJson(GetContextInJson());
		clonedMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
	}

	return clonedMessage;
}

int MeteoContextBluetoothMessage::SetAccessType(MeteoBluetoothMessageAccessType aType)
{
	if (accessType == MeteoBluetoothMessageAccessType::WriteOnly) {
		contextInJson.clear();
		if (contextSize > 0) {
			delete[] context;
			contextSize = 0;
		}

	}
	else if (accessType == MeteoBluetoothMessageAccessType::ReadOnly) {
		if (contextInJson.size() > 0) {
			string jsonDump = contextInJson.dump();

			context = new char[jsonDump.length() + 1];
			strcpy(context, jsonDump.c_str());
			contextSize = jsonDump.length() + 1;
		}
		else if (contextSize > 0) {
			contextInJson.parse(context);
		}

	}

	accessType = aType;

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

		contextInJson.clear();

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

		if (contextSize != 0) {
			delete[] context;
			contextSize = 0;
		}
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

bool MeteoContextBluetoothMessage::GetIsRawMessage()
{

	unsigned int c = (unsigned int)command;

	switch (c & 0x000F0000) {
	case 0x00000000:	//con
	case 0x00010000:	//non
		return false;
		break;
	case 0x00020000:	//ack
	case 0x00030000:	//retcon
	case 0x00040000:	//retnon
		return true;
		break;
	}

	return true;
}
