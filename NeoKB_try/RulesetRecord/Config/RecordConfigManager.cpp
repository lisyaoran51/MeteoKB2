#include "RecordConfigManager.h"


using namespace Record::Config;
using namespace Framework::Configurations;


RecordConfigManager::RecordConfigManager(): ConfigManager(), RegisterType("RecordConfigManager")
{
}

int RecordConfigManager::InitializeDefault()
{
	Set(RecordSetting::PatternGenerator, "RecordPatternGenerator");
	return 0;
}
