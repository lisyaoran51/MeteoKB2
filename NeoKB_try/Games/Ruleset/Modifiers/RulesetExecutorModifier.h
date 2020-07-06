#ifndef RULESET_EXECUTOR_MODIFIER_H
#define RULESET_EXECUTOR_MODIFIER_H


#include "../RulesetExecutor.h"
#include "Modifier.h"


using namespace Games::Rulesets;

namespace Games {
namespace Rulesets{
namespace Modifiers{

	template<typename T>
	class RulesetExecutorModifier: virtual public Modifier {

	public:

		virtual int ApplyToRulesetContainer(RulesetExecutor<T>* rulesetExecutor) = 0;

	};

}}}


#endif