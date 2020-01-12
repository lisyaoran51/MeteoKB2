#ifndef I_APPLICABLE_TO_RULESET_EXECUTOR_H
#define I_APPLICABLE_TO_RULESET_EXECUTOR_H

namespace Games {
namespace Rulesets{
namespace Modifiers{

	template<typename T>
	class IApplicableToRulesetExecutor {

	public:

		virtual int ApplyToRulesetContainer(RulesetExecutor<T>* rulesetExecutor) = 0;

	};

}}}


#endif