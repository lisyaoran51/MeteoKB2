#include "HandModifier.h"

using namespace Meteor::Rulesets::Modifiers;


HandModifier::HandModifier(SmDifficultyHandType hType) : RegisterType("HandModifier")
{
	handType = hType;
}

int HandModifier::ApplyToDifficulty(SmDifficulty * smDifficulty)
{
	smDifficulty->HandType = handType;
	return 0;
}
