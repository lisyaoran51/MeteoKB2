#ifndef HAND_MODIFIER_H
#define HAND_MODIFIER_H


#include "../../../Games/Ruleset/Modifiers/Modifier.h"
#include "../../../Games/Ruleset/Modifiers/DifficultyModifier.h"
#include "../../../Games/Ruleset/Modifiers/EventProcessorFilterModifier.h"
#include "../../../Games/Scheduler/Event/EventProcessor.h"


using namespace Games::Rulesets::Modifiers;
using namespace Games::Schedulers::Events;

namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	class HandModifier : public DifficultyModifier, public EventProcessorFilterModifier {

	public:

		HandModifier();

		HandModifier(SmDifficultyHandType hType);

		virtual int ApplyToDifficulty(SmDifficulty* smDifficulty);

		virtual int ApplyToEventProcessorFilter(EventProcessorFilter* eventProcessorFilter);

		virtual int SetValue(int value1, int value2);

	protected:

		SmDifficultyHandType handType = SmDifficultyHandType::None;

		int filterEventProcessorsByHandType(vector<EventProcessor<Event>*>* eventProcessors);

	};



}}}





#endif