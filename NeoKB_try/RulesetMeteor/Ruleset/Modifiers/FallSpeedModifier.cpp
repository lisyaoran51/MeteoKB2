#include "FallSpeedModifier.h"


using namespace Meteor::Rulesets::Modifiers;


FallSpeedModifier::FallSpeedModifier(float fSpeed) : RegisterType("FallSpeedModifier")
{
	fallSpeed = fSpeed;
}

int FallSpeedModifier::ApplyToDifficulty(SmDifficulty * smDifficulty)
{
	smDifficulty->FallSpeed = fallSpeed;
	return 0;
}
