#include "MeteoGameDesktop.h"


using namespace Desktop;




MeteoGameDesktop::MeteoGameDesktop(vector<string>& args): RegisterType("MeteoGameDesktop")
{
}

Storage * MeteoGameDesktop::GetStableStorage()
{
	return new StableStorage();
}

MeteoGameDesktop::StableStorage::StableStorage(): PlatformStorage("")
{
}

string MeteoGameDesktop::StableStorage::locateBasePath()
{
	return string("/home/pi/Meteo");
}
