#include "MeteoGameDesktop.h"


using namespace Desktop;




MeteoGameDesktop::MeteoGameDesktop(vector<string>& args): RegisterType("MeteoGameDesktop")
{
}

Storage * MeteoGameDesktop::GetStableStorage()
{
	// �o�Ӥ���n�令������|
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
