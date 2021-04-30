#include "FrameworkConfigManager.h"

using namespace Framework::Configurations;


FrameworkConfigManager::FrameworkConfigManager(): TConfigManager(), RegisterType("FrameworkConfigManager")
{
}

int FrameworkConfigManager::InitializeDefault()
{

	Set(FrameworkSetting::FrameRate, 15);
	Set(FrameworkSetting::PeriodMapInterval, 1);
	return 0;
}
