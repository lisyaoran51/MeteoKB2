#include "RulesetStore.h"

#include <stdexcept>

using namespace Games::Rulesets;
using namespace std;

int RulesetStore::loadRulesetFromFile(string path)
{
	throw runtime_error("SheetmusicStore::Delete(): don't use this.");
	return 0;
}

RulesetStore::RulesetStore(function<DatabaseContext*(void)> gContext, Storage * s): DatabaseStore(gContext, s), RegisterType("RulesetStore")
{
}

vector<RulesetInfo*>* RulesetStore::GetRulesetInfos()
{
	DatabaseContext* context = getContext();

	return context->GetRulesetInfos()->GetEntities();
}

RulesetInfo * RulesetStore::GetRulesetInfo(int id)
{
	DatabaseContext* context = getContext();

	vector<RulesetInfo*>* infos = context->GetRulesetInfos()->GetEntitiesOfPropertiyInt("Id", id);
	if (infos->size > 0) {
		RulesetInfo* info = infos->at(0);
		delete infos;
		return info;
	}

	delete infos;
	return nullptr;
}
