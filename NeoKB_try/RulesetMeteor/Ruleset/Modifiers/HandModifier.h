#ifndef HAND_MODIFIER_H
#define HAND_MODIFIER_H


#include "../../../Games/Ruleset/Modifiers/Modifier.h"
#include "../../../Games/Ruleset/Modifiers/DifficultyModifier.h"
#include "../../../Games/Ruleset/Modifiers/EventProcessorFilterModifier.h"


using namespace Games::Rulesets::Modifiers;

namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	class HandModifier : public DifficultyModifier, public EventProcessorFilterModifier {

	public:

		HandModifier(SmDifficultyHandType hType);

		virtual int ApplyToDifficulty(SmDifficulty* smDifficulty);

		virtual int ApplyToEventProcessorFilter(EventProcessorFilter* eventProcessorFilter);

	protected:

		SmDifficultyHandType handType = SmDifficultyHandType::None;

	};



}}}





#endif