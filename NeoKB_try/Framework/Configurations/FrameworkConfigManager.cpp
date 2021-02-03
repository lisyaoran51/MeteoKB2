#include "FrameworkConfigManager.h"

using namespace Framework::Configurations;


FrameworkConfigManager::FrameworkConfigManager(): ConfigManager(), RegisterType("FrameworkConfigManager")
{
}

int FrameworkConfigManager::InitializeDefault()
{

	Set(FrameworkSetting::FrameRate, 15);
	Set(FrameworkSetting::PeriodMapInterval, 1);
	return 0;
}
