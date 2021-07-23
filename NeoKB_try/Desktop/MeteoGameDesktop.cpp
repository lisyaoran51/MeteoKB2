#include "MeteoGameDesktop.h"

#include <fstream>

using namespace Desktop;
using namespace std;



int MeteoGameDesktop::load()
{
	Storage* s = GetStableStorage();

	fstream* stream = s->GetStream("SystemData/HardwareInfo");

	string line;

	try
	{
		getline(*stream, line);
	}
	catch (const exception& e) //it would not work if you pass by value
	{
		line = "Unknown";
	}

	hardwareModelName = line;

	delete s;
	s = nullptr;

	return 0;
}

MeteoGameDesktop::MeteoGameDesktop(vector<string>& args): RegisterType("MeteoGameDesktop")
{
	isPresent = true;
	isInputable = true;
	registerLoad(bind((int(MeteoGameDesktop::*)())&MeteoGameDesktop::load, this));
}

Storage * MeteoGameDesktop::GetStableStorage()
{

	// 這個之後要改成絕對路徑，指向主程式以外的地方，不能跟主程式擺在一起
	Storage* s = new StableStorage("/home/pi");
	s->Initialize();

	return s;
}

int MeteoGameDesktop::onMessage(MeteoBluetoothMessage * message)
{
	MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);


	LOG(LogLevel::Depricated) << "MeteoGameDesktop::onMessage() : got new bt message. ";

	if (contextMessage->GetCommand() == MeteoCommand::ReadHardwareData) {

		if (hardwareModelName == "")
			return -1;

		OutputManager* o = GetCache<OutputManager>("OutputManager");

		if (!o) 
			return -1;

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::ReturnHardwareData);

		json returnContext;

		returnContext["Value"] = hardwareModelName;

		meteoContextBluetoothMessage->SetContextInJson(returnContext);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		o->PushMessage(meteoContextBluetoothMessage);

	}

	return 0;
}

MeteoGameDesktop::StableStorage::StableStorage(string rName): PlatformStorage(""), RegisterType("StableStorage")
{
	rootName = rName;
}

string MeteoGameDesktop::StableStorage::locateBasePath()
{
	return rootName;
}
