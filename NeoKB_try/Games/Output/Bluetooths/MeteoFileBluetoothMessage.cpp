#include "MeteoFileBluetoothMessage.h"


using namespace Games::Output::Bluetooths;


MeteoFileBluetoothMessage::MeteoFileBluetoothMessage(MeteoCommand mCommand, string fPath) : MeteoBluetoothMessage(mCommand)
{
	filePath = fPath;
}

MeteoFileBluetoothMessage::MeteoFileBluetoothMessage(MeteoCommand mCommand, string fPath, string fName) : MeteoBluetoothMessage(mCommand)
{
	filePath = fPath;
	fileName = fName;
}

string MeteoFileBluetoothMessage::GetFilePath()
{
	return filePath;
}

string MeteoFileBluetoothMessage::GetFileName()
{
	return fileName;
}
