#include "MeteoGameDesktop.h"


using namespace Desktop;




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
