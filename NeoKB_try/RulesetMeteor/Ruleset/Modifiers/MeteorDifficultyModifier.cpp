#include "MeteorDifficultyModifier.h"



using namespace Meteor::Rulesets::Modifiers;



MeteorDifficultyModifier::MeteorDifficultyModifier(SmDifficultyDifficulty d) :RegisterType("DifficultyModifier")
{
	difficulty = d;
}

int MeteorDifficultyModifier::ApplyToDifficulty(SmDifficulty * smDifficulty)
{
	smDifficulty->Difficulty = difficulty;

	return 0;
}
