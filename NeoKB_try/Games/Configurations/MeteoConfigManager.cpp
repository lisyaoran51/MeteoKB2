#include "MeteoConfigManager.h"


using namespace Games::Configurations



int MeteoConfigManager::InitializeDefault()
{

	Set(MeteoSetting::FrameRate, Ruleset);

	return 0;
}
