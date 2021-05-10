#include "FrameworkConfigManager.h"

using namespace Framework::Configurations;


FrameworkConfigManager::FrameworkConfigManager(): TConfigManager(), RegisterType("FrameworkConfigManager")
{
	fileName = "Framework.ini";
}

int FrameworkConfigManager::initializeDefault()
{
	Set(FrameworkSetting::FrameRate, 15);
	Set(FrameworkSetting::PeriodMapInterval, 1);
	return 0;
}

int FrameworkConfigManager::setNamesOfConfig()
{
	namesOfConfig["SongTitle"] = FrameworkSetting::SongTitle;
	namesOfConfig["AudioDevice"] = FrameworkSetting::AudioDevice;
	namesOfConfig["VolumeUniversal"] = FrameworkSetting::VolumeUniversal;
	namesOfConfig["VolumeEffect"] = FrameworkSetting::VolumeEffect;
	namesOfConfig["VolumeMusic"] = FrameworkSetting::VolumeMusic;

	namesOfConfig["Width"] = FrameworkSetting::Width;
	namesOfConfig["Height"] = FrameworkSetting::Height;
	namesOfConfig["TargetHeight"] = FrameworkSetting::TargetHeight;
	namesOfConfig["BlackKeyHeight"] = FrameworkSetting::BlackKeyHeight;
	namesOfConfig["BlackKeyTargetHeight"] = FrameworkSetting::BlackKeyTargetHeight;

	namesOfConfig["StartPitch"] = FrameworkSetting::StartPitch;

	namesOfConfig["FrameSync"] = FrameworkSetting::FrameSync;
	namesOfConfig["FrameRate"] = FrameworkSetting::FrameRate;
	namesOfConfig["UpdateRate"] = FrameworkSetting::UpdateRate;
	namesOfConfig["InputRate"] = FrameworkSetting::InputRate;
	namesOfConfig["AudioUpdateRate"] = FrameworkSetting::AudioUpdateRate;
	namesOfConfig["CursorSensitivity"] = FrameworkSetting::CursorSensitivity;

	namesOfConfig["PatternGenerator"] = FrameworkSetting::PatternGenerator;
	namesOfConfig["LedDriver"] = FrameworkSetting::LedDriver;
	namesOfConfig["HardwareVersion"] = FrameworkSetting::HardwareVersion;
	namesOfConfig["PeriodMapInterval"] = FrameworkSetting::PeriodMapInterval;
	return 0;
}
