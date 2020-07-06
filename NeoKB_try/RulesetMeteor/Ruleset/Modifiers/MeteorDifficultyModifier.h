#ifndef METEOR_DIFFICULTY_MODIFIER_H
#define METEOR_DIFFICULTY_MODIFIER_H





#include "../../../Games/Ruleset/Modifiers/Modifier.h"
#include "../../../Games/Ruleset/Modifiers/DifficultyModifier.h"


using namespace Games::Rulesets::Modifiers;

namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	class MeteorDifficultyModifier : public DifficultyModifier {

	public:

		MeteorDifficultyModifier(SmDifficultyDifficulty d);

		virtual int ApplyToDifficulty(SmDifficulty* smDifficulty);

	protected:

		SmDifficultyDifficulty difficulty = SmDifficultyDifficulty::None;

	};



}}}




#endif