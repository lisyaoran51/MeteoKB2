#include "RulesetInfo.h"
#include "../../Util/InstanceCreator.h"
#include "../../Framework/Allocation/Hierachal/MtoObject.h"
#include "Ruleset.h"


using namespace Base::Rulesets;
using namespace Framework::Allocation::Hierachal;


RulesetInfo::RulesetInfo(string n)
{
	name = n;
}

Ruleset* RulesetInfo::CreateRuleset()
{
	InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
	Ruleset* r = iCreator.CreateInstanceWithT<Ruleset>(name);
	r->LazyConstruct(this);
	return r;
}

int RulesetInfo::GetId()
{
	return id;
}

string RulesetInfo::GetTypeName()
{
	return name;
}
