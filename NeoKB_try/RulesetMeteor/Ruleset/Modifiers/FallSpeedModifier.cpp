#include "FallSpeedModifier.h"


using namespace Meteor::Rulesets::Modifiers;


FallSpeedModifier::FallSpeedModifier() : RegisterType("FallSpeedModifier")
{
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
