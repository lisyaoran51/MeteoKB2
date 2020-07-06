#ifndef HAND_MODIFIER_H
#define HAND_MODIFIER_H


#include "../../../Games/Ruleset/Modifiers/Modifier.h"
#include "../../../Games/Ruleset/Modifiers/DifficultyModifier.h"


using namespace Games::Rulesets::Modifiers;

namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	class HandModifier : virtual public DifficultyModifier {

	public:

		HandModifier(SmDifficultyHandType hType);

		virtual int ApplyToDifficulty(SmDifficulty* smDifficulty);

	protected:

		SmDifficultyHandType handType = SmDifficultyHandType::None;

	};



}}}





#endif