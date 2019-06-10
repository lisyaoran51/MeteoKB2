#include "FrameworkConfigManager.h"

using namespace Base::Config;


FrameworkConfigManager::FrameworkConfigManager(): ConfigManager(), RegisterType("FrameworkConfigManager")
{
}

int FrameworkConfigManager::InitializeDefault()
{

	Set(FrameworkSetting::FrameRate, 15);
	return 0;
}
