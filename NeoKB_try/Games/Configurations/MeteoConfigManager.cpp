#include "MeteoConfigManager.h"


using namespace Games::Configurations;



MeteoConfigManager::MeteoConfigManager(): RegisterType("MeteoConfigManager")
{
}

int MeteoConfigManager::InitializeDefault()
{

	Set(MeteoSetting::Ruleset, 0);

	Set(MeteoSetting::AudioOffset, -0.1f);

	return 0;
}
