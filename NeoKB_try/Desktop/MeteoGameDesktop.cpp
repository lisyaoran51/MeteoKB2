#include "MeteoGameDesktop.h"


using namespace Desktop;




MeteoGameDesktop::MeteoGameDesktop(vector<string>& args): RegisterType("MeteoGameDesktop")
{
}

Storage * MeteoGameDesktop::GetStableStorage()
{
	Storage* s = new StableStorage();
	s->Initialize();
	return s;
}

MeteoGameDesktop::StableStorage::StableStorage(): PlatformStorage(""), RegisterType("StableStorage")
{
}

string MeteoGameDesktop::StableStorage::locateBasePath()
{
	return string("./");
}
