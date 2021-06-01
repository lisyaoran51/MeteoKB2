#include "MeteoAckFileSegmentBluetoothMessage.h"

#include "../../../Util/Log.h"


using namespace Games::Output::Bluetooths;
using namespace Util;


MeteoAckFileSegmentBluetoothMessage::MeteoAckFileSegmentBluetoothMessage(MeteoCommand mCommand, string fName, int o, int a) : MeteoBluetoothMessage(mCommand)
{
	fileName = fName;
	order = o;
	amount = a;
	accessType = MeteoBluetoothMessageAccessType::ReadOnly;
}

BluetoothMessage * MeteoAckFileSegmentBluetoothMessage::Clone()
{
	return nullptr;
}

int MeteoAckFileSegmentBluetoothMessage::SetContext(char * c, int cSize)
{
	LOG(LogLevel::Error) << "MeteoAckFileSegmentBluetoothMessage::SetContext() : unavailable to do this.";
	return 0;
}

int MeteoAckFileSegmentBluetoothMessage::SetAccessType(MeteoBluetoothMessageAccessType aType)
{
	LOG(LogLevel::Error) << "MeteoAckFileSegmentBluetoothMessage::SetAccessType() : unavailable to do this.";
	return 0;
}

string MeteoAckFileSegmentBluetoothMessage::GetFileName()
{
	return fileName;
}

int MeteoAckFileSegmentBluetoothMessage::GetOrder()
{
	return order;
}

int MeteoAckFileSegmentBluetoothMessage::GetAmount()
{
	return amount;
}

bool MeteoAckFileSegmentBluetoothMessage::GetIsRawMessage()
{
	return true;
}
