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

	// �]��instance creator�S��k�b�إߪ��ɭԴN��Ѽƥ�i�h? 
	// �S��k�@�}�l��A�ҥH�N����A��lazy construct �A�o�ˤ���w��
	rulesetExecutor->LazyConstruct(sm);

	return rulesetExecutor;
}
