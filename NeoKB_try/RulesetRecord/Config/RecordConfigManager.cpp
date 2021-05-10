#include "RecordConfigManager.h"


using namespace Record::Config;
using namespace Framework::Configurations;


RecordConfigManager::RecordConfigManager(): TConfigManager(), RegisterType("RecordConfigManager")
{
	fileName = "RulesetRecord.ini";
}

int RecordConfigManager::setNamesOfConfig()
{
	namesOfConfig["PatternGenerator"] = RecordSetting::PatternGenerator;
	namesOfConfig["RecordSystemController"] = RecordSetting::RecordSystemController;
	return 0;
}

int RecordConfigManager::initializeDefault()
{
	Set(RecordSetting::PatternGenerator, "RecordPatternGenerator");
	return 0;
}
