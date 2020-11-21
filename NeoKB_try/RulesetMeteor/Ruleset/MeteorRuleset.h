#ifndef METEOR_RULESET_H
#define METEOR_RULESET_H

#include "../../Games/Ruleset/Ruleset.h"

using namespace Games::Rulesets;

namespace Meteor {
namespace Rulesets{

	class MeteorRuleset: public Ruleset{

		bool constructed;

	public:

		MeteorRuleset();

		MeteorRuleset(RulesetInfo* r);

		virtual int LazyConstruct(RulesetInfo* r);

		RulesetExecutor<Event>* CreateRulesetExecutor(WorkingSm* sm);

		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0);

		virtual PitchBindingSet* GetDefaultPitchBindingSet(int variant = 0);

	};

}
}








#endif