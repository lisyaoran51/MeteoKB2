#include"MeteorRuleset.h"
#include "MeteorRulesetExecutor.h"

using namespace Meteor::Rulesets;

MeteorRuleset::MeteorRuleset() : RegisterType("Ruleset")
{
	constructed = false;
}

MeteorRuleset::MeteorRuleset(RulesetInfo * r): RegisterType("Ruleset")
{
	rulesetInfo = r;
	constructed = true;
}

int MeteorRuleset::LazyConstruct(RulesetInfo * r)
{
	Ruleset::LazyConstruct(r);
	constructed = true;
	return 0;
}

RulesetExecutor<Event>* MeteorRuleset::CreateRulesetExecutor(WorkingSm * sm)
{
	InstanceCreator<MtoObject>& iCreator = InstanceCreator<MtoObject>::GetInstance();

	RulesetExecutor<Event>* rulesetExecutor = iCreator.CreateInstanceWithT<MeteorRulesetExecutor>("MeteorRulesetExecutor");

	// instance creator⊿快猭ミ碞р把计メ秈? 
	// ⊿快猭秨﹍メ┮碞ぇㄓlazy construct 硂妓ゑ耕
	rulesetExecutor->LazyConstruct(sm);

	return rulesetExecutor;
}
