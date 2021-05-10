#include "MeteorConfigManager.h"


using namespace Meteor::Config;
using namespace Framework::Configurations;


MeteorConfigManager::MeteorConfigManager(): TConfigManager(), RegisterType("MeteorConfigManager")
{
	fileName = "RulesetMeteor.ini";
}

int MeteorConfigManager::initializeDefault()
{
	Set(MeteorSetting::PatternGenerator, "MeteorPatternGenerator");
	Set(MeteorSetting::FallMapAlgorithm, "FallMapAlgorithm");
	Set(MeteorSetting::ExplodeMapAlgorithm, "ExplodeMapAlgorithm");
	Set(MeteorSetting::GlowLineMapAlgorithm, "GlowLineMapAlgorithm");
	Set(MeteorSetting::TargetLineMapAlgorithm, "TargetLineMapAlgorithm");
	Set(MeteorSetting::SystemController, "SystemController");

	Set(MeteorSetting::FallSpeed, MTO_FLOAT(16.f));
	Set(MeteorSetting::FallBrightness, MTO_FLOAT(1.f));
	Set(MeteorSetting::FallLength, 8);
	Set(MeteorSetting::ExplodeSpeed, MTO_FLOAT(16.f));
	Set(MeteorSetting::ExplodeBrightness, MTO_FLOAT(1.f));
	Set(MeteorSetting::ExplodeLifeTime, MTO_FLOAT(0.1f));
	Set(MeteorSetting::GlowLineSpeed, MTO_FLOAT(1.f));
	Set(MeteorSetting::GlowLineBrightness, MTO_FLOAT(1.f));
	Set(MeteorSetting::GlowLineDuration, MTO_FLOAT(1.f));

	Set(MeteorSetting::TargetLineBrightness, MTO_FLOAT(1.f));

	Set(MeteorSetting::RestartSection, 0);
	return 0;
}

int MeteorConfigManager::setNamesOfConfig()
{
	namesOfConfig["PatternGenerator"] = MeteorSetting::PatternGenerator;
	namesOfConfig["FallMapAlgorithm"] = MeteorSetting::FallMapAlgorithm;
	namesOfConfig["EruptMapAlgorithm"] = MeteorSetting::EruptMapAlgorithm;
	namesOfConfig["ExplodeMapAlgorithm"] = MeteorSetting::ExplodeMapAlgorithm;
	namesOfConfig["GlowLineMapAlgorithm"] = MeteorSetting::GlowLineMapAlgorithm;
	namesOfConfig["TargetLineMapAlgorithm"] = MeteorSetting::TargetLineMapAlgorithm;
	namesOfConfig["SustainPedalLightRingIoCommunicator"] = MeteorSetting::SustainPedalLightRingIoCommunicator;
	namesOfConfig["InstrumentController"] = MeteorSetting::InstrumentController;
	namesOfConfig["VirtualInstrumentController"] = MeteorSetting::VirtualInstrumentController;
	namesOfConfig["RepeatPracticeController"] = MeteorSetting::RepeatPracticeController;
	namesOfConfig["SystemController"] = MeteorSetting::SystemController;
	namesOfConfig["RecorderController"] = MeteorSetting::RecorderController;

	namesOfConfig["MapPitchShifter"] = MeteorSetting::MapPitchShifter;
	namesOfConfig["OctaveShifter"] = MeteorSetting::OctaveShifter;
	namesOfConfig["PitchShiftSpeed"] = MeteorSetting::PitchShiftSpeed;

	namesOfConfig["WhiteKeyTargetHeight"] = MeteorSetting::WhiteKeyTargetHeight;
	namesOfConfig["BlackKeyTargetHeight"] = MeteorSetting::BlackKeyTargetHeight;

	namesOfConfig["FallSpeed"] = MeteorSetting::FallSpeed;
	namesOfConfig["FallBrightness"] = MeteorSetting::FallBrightness;
	namesOfConfig["FallLength"] = MeteorSetting::FallLength;
	namesOfConfig["ExplodeSpeed"] = MeteorSetting::ExplodeSpeed;
	namesOfConfig["ExplodeBrightness"] = MeteorSetting::ExplodeBrightness;
	namesOfConfig["ExplodeLifeTime"] = MeteorSetting::ExplodeLifeTime;
	namesOfConfig["GlowLineSpeed"] = MeteorSetting::GlowLineSpeed;
	namesOfConfig["GlowLineDuration"] = MeteorSetting::GlowLineDuration;
	namesOfConfig["GlowLineBrightness"] = MeteorSetting::GlowLineBrightness;
	namesOfConfig["TargetLineBlinkSpeed"] = MeteorSetting::TargetLineBlinkSpeed;
	namesOfConfig["TargetLineBrightness"] = MeteorSetting::TargetLineBrightness;

	namesOfConfig["RestartSection"] = MeteorSetting::RestartSection;
	return 0;
}
