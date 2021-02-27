#ifndef RECORD_RULESET_H
#define RECORD_RULESET_H

#include "../../Games/Ruleset/Ruleset.h"

using namespace Games::Rulesets;

namespace Record {
namespace Rulesets{

	class RecordRuleset: public Ruleset{

		bool constructed;

	public:

		RecordRuleset();

		RecordRuleset(RulesetInfo* r);

		virtual int LazyConstruct(RulesetInfo* r);

		RulesetExecutor<Event>* CreateRulesetExecutor(WorkingSm* sm);

		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0);

		virtual PitchBindingSet* GetDefaultPitchBindingSet(int variant = 0);

	};

}
}








#endif