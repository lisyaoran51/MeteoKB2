#include "InstrumentConfigManager.h"


using namespace Instruments::Configurations;



InstrumentConfigManager::InstrumentConfigManager() : RegisterType("InstrumentConfigManager")
{
	fileName = "Instrument.ini";
}

int InstrumentConfigManager::initializeDefault()
{
	Set(InstrumentSetting::InitialSustainType, 1);

	Set(InstrumentSetting::SensitiveLevel, 2);

	Set(InstrumentSetting::InitialSoundBankName, "U3");
	return 0;
}

int InstrumentConfigManager::setNamesOfConfig()
{
	namesOfConfig["InitialSustainType"] = InstrumentSetting::InitialSustainType;
	namesOfConfig["SensitiveLevel"] = InstrumentSetting::SensitiveLevel;
	namesOfConfig["InitialSoundBankName"] = InstrumentSetting::InitialSoundBankName;
	return 0;
}
