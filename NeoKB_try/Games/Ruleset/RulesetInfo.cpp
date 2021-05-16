#include "RulesetInfo.h"
#include "../../Util/InstanceCreator.h"
#include "../../Framework/Allocation/Hierachal/MtoObject.h"
#include "Ruleset.h"


using namespace Games::Rulesets;
using namespace Framework::Allocation::Hierachal;


RulesetInfo::RulesetInfo(string n, int i)
{
	name = n;
	id = i;
}

Ruleset* RulesetInfo::CreateRuleset()
{
	InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
	LOG(LogLevel::Fine) << "RulesetInfo::CreateRuleset() : create ruleset [" << name << "].";

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
