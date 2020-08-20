#include "InstantConfigManager.h"


using namespace Instant::Config;
using namespace Framework::Configurations;


InstantConfigManager::InstantConfigManager(): ConfigManager(), RegisterType("InstantConfigManager")
{
}

int InstantConfigManager::InitializeDefault()
{
	Set(InstantSetting::PatternGenerator, "MeteorPatternGenerator");
	Set(InstantSetting::FallMapAlgorithm, "FallMapAlgorithm");

	Set(InstantSetting::FallSpeed, MTO_FLOAT(16.f));
	Set(InstantSetting::FallBrightness, MTO_FLOAT(1.f));
	Set(InstantSetting::FallLength, 8);
	return 0;
}
