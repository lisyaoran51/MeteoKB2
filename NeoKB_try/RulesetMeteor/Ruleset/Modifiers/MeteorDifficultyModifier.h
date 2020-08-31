#ifndef METEOR_DIFFICULTY_MODIFIER_H
#define METEOR_DIFFICULTY_MODIFIER_H





#include "../../../Games/Ruleset/Modifiers/Modifier.h"
#include "../../../Games/Ruleset/Modifiers/DifficultyModifier.h"
#include "../../../Games/Ruleset/Modifiers/EventProcessorFilterModifier.h"
#include "../../../Games/Scheduler/Event/EventProcessor.h"



using namespace Games::Rulesets::Modifiers;

namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	class MeteorDifficultyModifier : public DifficultyModifier, public EventProcessorFilterModifier {

	public:

		MeteorDifficultyModifier();

		MeteorDifficultyModifier(SmDifficultyDifficulty d);

		virtual int ApplyToDifficulty(SmDifficulty* smDifficulty);

		virtual int ApplyToEventProcessorFilter(EventProcessorFilter* eventProcessorFilter);

		virtual int SetValue(int value1, int value2);

	protected:

		SmDifficultyDifficulty difficulty = SmDifficultyDifficulty::None;

		int filterEventProcessorsByDifficulty(vector<EventProcessor<Event>*>* eventProcessors);



	};



}}}




#endif