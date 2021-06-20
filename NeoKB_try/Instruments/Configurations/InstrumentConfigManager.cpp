#include "InstrumentConfigManager.h"


using namespace Instruments::Configurations;



InstrumentConfigManager::InstrumentConfigManager() : RegisterType("InstrumentConfigManager")
{
	fileName = "Instrument.ini";
}

int InstrumentConfigManager::initializeDefault()
{
	Set(InstrumentSetting::InitialSustainType, 2);

	Set(InstrumentSetting::SensitiveLevel, 2);
	return 0;
}

int InstrumentConfigManager::setNamesOfConfig()
{
	namesOfConfig["InitialSustainType"] = InstrumentSetting::InitialSustainType;
	namesOfConfig["SensitiveLevel"] = InstrumentSetting::SensitiveLevel;
	return 0;
}
