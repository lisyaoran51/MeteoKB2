#ifndef AUTO_PEDAL_MODIFIER_H
#define AUTO_PEDAL_MODIFIER_H


#include "../../../Games/Ruleset/Modifiers/Modifier.h"
#include "../../../Games/Ruleset/Modifiers/IApplicableToRulesetExecutor.h"
#include "../../../Games/Scheduler/Event/Event.h"


using namespace Games::Rulesets::Modifiers;
using namespace Games::Schedulers::Events;


namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	class AutoPedalModifier : public Modifier, public IApplicableToRulesetExecutor<Event> {

	public:

		virtual int ApplyToRulesetContainer(RulesetExecutor<Event>* rulesetExecutor);


	};

}}}




#endif