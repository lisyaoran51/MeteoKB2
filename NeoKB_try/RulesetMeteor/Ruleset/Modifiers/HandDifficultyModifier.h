#ifndef HAND_DIFFICULTY_MODIFIER_H
#define HAND_DIFFICULTY_MODIFIER_H


#include "../../../Games/Ruleset/Modifiers/Modifier.h"
#include "../../../Games/Ruleset/Modifiers/DifficultyModifier.h"
#include "../../../Games/Ruleset/Modifiers/EventProcessorFilterModifier.h"
#include "../../../Games/Scheduler/Event/EventProcessor.h"


using namespace Games::Rulesets::Modifiers;
using namespace Games::Schedulers::Events;

namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	class HandDifficultyModifier : public DifficultyModifier, public EventProcessorFilterModifier {

	public:

		HandDifficultyModifier();

		HandDifficultyModifier(SmDifficultyHandType hType, SmDifficultyDifficulty d);

		virtual int ApplyToDifficulty(SmDifficulty* smDifficulty);

		virtual int ApplyToEventProcessorFilter(EventProcessorFilter* eventProcessorFilter);

		virtual int SetValue(float value1, float value2);

		SmDifficultyHandType GetHandType();

		SmDifficultyDifficulty GetDifficulty();

	protected:

		SmDifficultyHandType handType = SmDifficultyHandType::None;

		SmDifficultyDifficulty difficulty = SmDifficultyDifficulty::None;

		bool filterEventProcessorsByHandTypeAndDifficulty(EventProcessor<Event>* eventProcessor);

	};



}}}





#endif