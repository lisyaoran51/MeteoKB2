#include "MeteoFileSegmentBluetoothMessage.h"

#include <string.h>


using namespace Games::Output::Bluetooths;


MeteoFileSegmentBluetoothMessage::MeteoFileSegmentBluetoothMessage(MeteoCommand mCommand, char * fSegment, int fSegmentSize, string fName, int o, int a): MeteoBluetoothMessage(mCommand)
{

	fileSegment = new char[fSegmentSize];
	memcpy(fileSegment, fSegment, fSegmentSize);
	fileSegmentSize = fSegmentSize;

	fileName = fName;

	order = o;

	amount = a;

	accessType = MeteoBluetoothMessageAccessType::ReadOnly;

}

BluetoothMessage * MeteoFileSegmentBluetoothMessage::Clone()
{
	return new MeteoFileSegmentBluetoothMessage(command, fileSegment, fileSegmentSize, fileName, order, amount);
}

int MeteoFileSegmentBluetoothMessage::SetContext(char * c, int cSize)
{
	// 不能使用這個功能
	return 0;
}

int MeteoFileSegmentBluetoothMessage::SetAccessType(MeteoBluetoothMessageAccessType aType)
{
	// 不能使用這個功能
	return 0;
}

string MeteoFileSegmentBluetoothMessage::GetFileName()
{
	return fileName;
}

char * MeteoFileSegmentBluetoothMessage::GetFileSegment()
{
	return fileSegment;
}

int MeteoFileSegmentBluetoothMessage::GetFileSegment(char ** fSegment)
{
	*fSegment = new char[fileSegmentSize];
	memcpy(*fSegment, fileSegment, fileSegmentSize);
	return 0;
}

int MeteoFileSegmentBluetoothMessage::GetFileSegmentSize()
{
	return fileSegmentSize;
}

int MeteoFileSegmentBluetoothMessage::GetOrder()
{
	return order;
}

int MeteoFileSegmentBluetoothMessage::GetAmount()
{
	return amount;
}

bool MeteoFileSegmentBluetoothMessage::GetIsRawMessage()
{
	return true;
}
