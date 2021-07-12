#include "FallSpeedModifier.h"


using namespace Meteor::Rulesets::Modifiers;


FallSpeedModifier::FallSpeedModifier() : RegisterType("FallSpeedModifier")
{
	fallSpeed = 16;
}

FallSpeedModifier::FallSpeedModifier(float fSpeed) : RegisterType("FallSpeedModifier")
{
	fallSpeed = fSpeed;
	throw runtime_error("FallSpeedModifier::FallSpeedModifier() : not implemented.");
}

int FallSpeedModifier::ApplyToDifficulty(SmDifficulty * smDifficulty)
{
	smDifficulty->Speed = fallSpeed;
	return 0;
}

int FallSpeedModifier::SetValue(float value1, float value2)
{
	if (value1 >= 16 && value1 <= 32)
		fallSpeed = value1;
	return 0;
}
