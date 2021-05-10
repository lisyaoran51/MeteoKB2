#include "MeteoConfigManager.h"


using namespace Games::Configurations;



MeteoConfigManager::MeteoConfigManager(): RegisterType("MeteoConfigManager")
{
	fileName = "Meteo.ini";
}

int MeteoConfigManager::initializeDefault()
{

	Set(MeteoSetting::Ruleset, 0);

	Set(MeteoSetting::AudioOffset, 0.2f);

	return 0;
}

int MeteoConfigManager::setNamesOfConfig()
{
	namesOfConfig["Ruleset"] = MeteoSetting::Ruleset;
	namesOfConfig["Username"] = MeteoSetting::Username;
	namesOfConfig["AudioOffset"] = MeteoSetting::AudioOffset;
	namesOfConfig["DefaultPianoVolume"] = MeteoSetting::DefaultPianoVolume;
	return 0;
}
