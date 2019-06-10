#include "MeteorConfigManager.h"


using namespace Meteor::Config;
using namespace Base::Config;


MeteorConfigManager::MeteorConfigManager(): ConfigManager(), RegisterType("MeteorConfigManager")
{
}

int MeteorConfigManager::InitializeDefault()
{
	Set(MeteorSetting::PatternGenerator, "MeteorPatternGenerator");
	Set(MeteorSetting::FallMapAlgorithm, "FallMapAlgorithm");
	Set(MeteorSetting::ExplodeMapAlgorithm, "ExplodeMapAlgorithm");
	Set(MeteorSetting::GlowLineMapAlgorithm, "GlowLineMapAlgorithm");
	Set(MeteorSetting::TargetLineMapAlgorithm, "TargetLineMapAlgorithm");

	Set(MeteorSetting::FallSpeed, MTO_FLOAT(16.f));
	Set(MeteorSetting::FallBrightness, MTO_FLOAT(1.f));
	Set(MeteorSetting::FallLength, 8);
	Set(MeteorSetting::ExplodeSpeed, MTO_FLOAT(16.f));
	Set(MeteorSetting::ExplodeBrightness, MTO_FLOAT(1.f));
	Set(MeteorSetting::GlowLineSpeed, MTO_FLOAT(1.f));
	Set(MeteorSetting::GlowLineBrightness, MTO_FLOAT(1.f));
	Set(MeteorSetting::GlowLineDuration, MTO_FLOAT(1.f));

	Set(MeteorSetting::RestartSection, 0);
	return 0;
}
