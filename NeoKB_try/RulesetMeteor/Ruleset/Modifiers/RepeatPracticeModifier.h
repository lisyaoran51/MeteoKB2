#ifndef REPEAT_PRACTICE_MODIFIER_H
#define REPEAT_PRACTICE_MODIFIER_H




#include "../../../Games/Ruleset/Modifiers/Modifier.h"
#include "../../../Games/Ruleset/Modifiers/TimeControllerModifier.h"
#include "../../../Games/Scheduler/Event/Event.h"


using namespace Games::Rulesets::Modifiers;
using namespace Games::Schedulers::Events;

namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	class RepeatPracticeModifier : public TimeControllerModifier {

	public:

		RepeatPracticeModifier(int rSection, int rTimes);

		int ApplyToTimeController(TimeController* timeController);

	protected:

		int repeatSections = 1;

		int repeatTimes = 4;

	};



}}}



#endif