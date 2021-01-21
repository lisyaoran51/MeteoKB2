#include "MeteoFileSegmentBluetoothMessage.h"


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
	// ����ϥγo�ӥ\��
	return 0;
}

int MeteoFileSegmentBluetoothMessage::SetAccessType(MeteoBluetoothMessageAccessType aType)
{
	// ����ϥγo�ӥ\��
	return 0;
}

string MeteoFileSegmentBluetoothMessage::GetFileName()
{
	return fileName;
}

int MeteoFileSegmentBluetoothMessage::GetFileSegment(char ** fSegment)
{
	*fSegment = new char[fileSegmentSize];
	memcpy(*fSegment, fileSegment, fileSegmentSize);
	return 0;
}

int MeteoFileSegmentBluetoothMessage::GetOrder()
{
	return order;
}

int MeteoFileSegmentBluetoothMessage::GetAmount()
{
	return amount;
}