#include "WhiteKeyTargetLineModifier.h"


using namespace Meteor::Rulesets::Modifiers;


WhiteKeyTargetLineModifier::WhiteKeyTargetLineModifier() : RegisterType("WhiteKeyTargetLineModifier")
{
}

string WhiteKeyTargetLineModifier::GetConfigManagerName()
{
	return "MeteorConfigManager";
}

int WhiteKeyTargetLineModifier::SetValue(float value1, float value2)
{
	whiteKeyTargetHeight = (int)value1;

	return 0;
}

int WhiteKeyTargetLineModifier::implementApplyToConfigurationManager(MeteorConfigManager * configManager)
{

	configManager->Set(MeteorSetting::WhiteKeyTargetHeight, whiteKeyTargetHeight);

	return 0;
}
