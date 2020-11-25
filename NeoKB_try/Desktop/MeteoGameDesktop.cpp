#include "MeteoGameDesktop.h"


using namespace Desktop;




MeteoGameDesktop::MeteoGameDesktop(vector<string>& args): RegisterType("MeteoGameDesktop")
{
}

Storage * MeteoGameDesktop::GetStableStorage()
{
	// 這個之後要改成絕對路徑，指向主程式以外的地方，不能跟主程式擺在一起
	Storage* s = new StableStorage(".");
	s->Initialize();

	return s;
}

MeteoGameDesktop::StableStorage::StableStorage(string rName): PlatformStorage(""), RegisterType("StableStorage")
{
	rootName = rName;
}

string MeteoGameDesktop::StableStorage::locateBasePath()
{
	return rootName;
}
