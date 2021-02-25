#ifndef INSTANT_RULESET_H
#define INSTANT_RULESET_H

#include "../../Games/Ruleset/Ruleset.h"

using namespace Games::Rulesets;

namespace Instant {
namespace Rulesets{

	class InstantRuleset: public Ruleset{

		bool constructed;

	public:

		InstantRuleset();

		InstantRuleset(RulesetInfo* r);

		virtual int LazyConstruct(RulesetInfo* r);

		RulesetExecutor<Event>* CreateRulesetExecutor(WorkingSm* sm);

		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0);

		virtual PitchBindingSet* GetDefaultPitchBindingSet(int variant = 0);

	};

}
}








#endif