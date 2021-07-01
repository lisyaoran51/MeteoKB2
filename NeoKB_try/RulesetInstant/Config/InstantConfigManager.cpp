#include "InstantConfigManager.h"


using namespace Instant::Config;
using namespace Framework::Configurations;


InstantConfigManager::InstantConfigManager(): TConfigManager(), RegisterType("InstantConfigManager")
{
	fileName = "RulesetInstant.ini";
}

int InstantConfigManager::initializeDefault()
{
	Set(InstantSetting::PatternGenerator, "MeteorPatternGenerator");
	Set(InstantSetting::InstantFallMapAlgorithm, "InstantFallMapAlgorithm");
	Set(InstantSetting::InstantGlowLineMapAlgorithm, "InstantGlowLineMapAlgorithm");
	Set(InstantSetting::InstantSpotMapAlgorithm, "InstantSpotMapAlgorithm");
	Set(InstantSetting::InstantVirtualInstrumentController, "InstantVirtualPianoController");
	Set(InstantSetting::InstantSystemController, "InstantSystemController");

	Set(InstantSetting::FallSpeed, MTO_FLOAT(16.f));
	Set(InstantSetting::FallBrightness, MTO_FLOAT(1.f));
	Set(InstantSetting::FallLength, 8);
	return 0;
}

int InstantConfigManager::setNamesOfConfig()
{
	namesOfConfig["PatternGenerator"] = InstantSetting::PatternGenerator;
	namesOfConfig["InstantFallMapAlgorithm"] = InstantSetting::InstantFallMapAlgorithm;
	namesOfConfig["InstantGlowLineMapAlgorithm"] = InstantSetting::InstantGlowLineMapAlgorithm;
	namesOfConfig["InstantVirtualInstrumentController"] = InstantSetting::InstantVirtualInstrumentController;
	namesOfConfig["InstantSystemController"] = InstantSetting::InstantSystemController;
	namesOfConfig["WhiteKeyTargetHeight"] = InstantSetting::WhiteKeyTargetHeight;
	namesOfConfig["BlackKeyTargetHeight"] = InstantSetting::BlackKeyTargetHeight;
	namesOfConfig["FallSpeed"] = InstantSetting::FallSpeed;
	namesOfConfig["FallBrightness"] = InstantSetting::FallBrightness;
	namesOfConfig["FallLength"] = InstantSetting::FallLength;

	return 0;
}
