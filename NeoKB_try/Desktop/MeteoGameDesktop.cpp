#include "MeteoGameDesktop.h"

#include <fstream>

using namespace Desktop;
using namespace std;



int MeteoGameDesktop::load()
{

	fstream* stream = GetStableStorage()->GetStream("SystemData/HardwareInfo");

	string line;

	try
	{
		getline(*stream, line);
	}
	catch (const exception& e) //it would not work if you pass by value
	{
		line = "Unknown";
	}


	return 0;
}

MeteoGameDesktop::MeteoGameDesktop(vector<string>& args): RegisterType("MeteoGameDesktop")
{
	registerLoad(bind((int(MeteoGameDesktop::*)())&MeteoGameDesktop::load, this));
}

Storage * MeteoGameDesktop::GetStableStorage()
{
	if (stableStorage == nullptr) {

		// 這個之後要改成絕對路徑，指向主程式以外的地方，不能跟主程式擺在一起
		Storage* s = new StableStorage("/home/pi");
		s->Initialize();
		stableStorage = s;

		return s;

	}
	else {
		return stableStorage;
	}

}

MeteoGameDesktop::StableStorage::StableStorage(string rName): PlatformStorage(""), RegisterType("StableStorage")
{
	rootName = rName;
}

string MeteoGameDesktop::StableStorage::locateBasePath()
{
	return rootName;
}
