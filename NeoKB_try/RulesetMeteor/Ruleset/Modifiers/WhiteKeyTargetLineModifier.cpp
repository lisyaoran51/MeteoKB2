#include "WhiteKeyTargetLineModifier.h"


using namespace Meteor::Rulesets::Modifiers;


WhiteKeyTargetLineModifier::WhiteKeyTargetLineModifier() : RegisterType("WhiteKeyTargetLineModifier")
{
}

string WhiteKeyTargetLineModifier::GetConfigManagerName()
{
	return "MeteorConfigManager";
}

int WhiteKeyTargetLineModifier::SetValue(int value1, int value2)
{
	whiteKeyTargetHeight = value1;

	return 0;
}

int WhiteKeyTargetLineModifier::implementApplyToConfigurationManager(MeteorConfigManager * configManager)
{

	configManager->Set(MeteorSetting::WhiteKeyTargetHeight, whiteKeyTargetHeight);

	return 0;
}
