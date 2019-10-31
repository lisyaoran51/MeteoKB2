#include "MeteoConfigManager.h"


using namespace Games::Configurations;



int MeteoConfigManager::InitializeDefault()
{

	Set(MeteoSetting::Ruleset, 0);

	return 0;
}
