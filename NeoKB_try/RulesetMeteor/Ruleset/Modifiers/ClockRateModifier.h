#ifndef CLOCK_RATE_MODIFIER_H
#define CLOCK_RATE_MODIFIER_H




#include "../../../Games/Ruleset/Modifiers/Modifier.h"
#include "../../../Games/Ruleset/Modifiers/TimeControllerModifier.h"


using namespace Games::Rulesets::Modifiers;

namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	class ClockRateModifier : public TimeControllerModifier {

	public:

		ClockRateModifier();
		ClockRateModifier(float cRate);

		virtual int ApplyToTimeController(TimeController* timeController);

		virtual int SetValue(float value1, float value2);

	protected:

		float clockRate = 1.0;

	};



}}}





#endif