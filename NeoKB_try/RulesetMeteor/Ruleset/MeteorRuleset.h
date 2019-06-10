#ifndef METEOR_RULESET_H
#define METEOR_RULESET_H

#include "../../Base/Ruleset/Ruleset.h"

using namespace Base::Rulesets;

namespace Meteor {
namespace Rulesets{

	class MeteorRuleset: public Ruleset{

		bool constructed;

	public:

		MeteorRuleset();

		MeteorRuleset(RulesetInfo* r);

		virtual int LazyConstruct(RulesetInfo* r);

		RulesetExecutor<Event>* CreateRulesetExecutor(WorkingSm* sm);

	};

}
}








#endif