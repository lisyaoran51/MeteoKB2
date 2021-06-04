#ifndef FALL_SPEED_MODIFIER_H
#define FALL_SPEED_MODIFIER_H




#include "../../../Games/Ruleset/Modifiers/Modifier.h"
#include "../../../Games/Ruleset/Modifiers/DifficultyModifier.h"


using namespace Games::Rulesets::Modifiers;

namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	class FallSpeedModifier : public DifficultyModifier {

	public:

		FallSpeedModifier();
		FallSpeedModifier(float fSpeed);

		virtual int ApplyToDifficulty(SmDifficulty* smDifficulty);

	protected:

		float fallSpeed = -1;

	};



}}}





#endif