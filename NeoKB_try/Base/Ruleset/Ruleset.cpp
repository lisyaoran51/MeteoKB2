#include "Ruleset.h"

using namespace Base::Rulesets;



Ruleset::Ruleset(): RegisterType("Ruleset")
{
	constructed = false;
}

Ruleset::Ruleset(RulesetInfo * r): RegisterType("Ruleset")
{
	rulesetInfo = r;
	constructed = true;
}

int Ruleset::LazyConstruct(RulesetInfo * r)
{
	rulesetInfo = r;
	constructed = true;
	return 0;
}

Ruleset::~Ruleset()
{
}

