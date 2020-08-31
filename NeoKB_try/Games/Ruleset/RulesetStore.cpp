#include "RulesetStore.h"

#include <stdexcept>

using namespace Games::Rulesets;
using namespace std;

int RulesetStore::loadRulesetFromFile(string path)
{
	throw runtime_error("RulesetStore::loadRulesetFromFile(): don't use this. no db implemented.");
	return 0;
}

RulesetStore::RulesetStore(function<DatabaseContext*(void)> gContext, Storage * s): DatabaseStore(gContext, s), RegisterType("RulesetStore")
{
}

vector<RulesetInfo*>* RulesetStore::GetRulesetInfos()
{
	throw runtime_error("RulesetStore::GetRulesetInfos() : not implemented.");


	DatabaseContext* context = getContext();

	return context->GetRulesetInfos()->GetEntities();
}

int RulesetStore::SetRulesetInfo(int id, RulesetInfo * rulesetInfo)
{
	rulesetInfos[id] = rulesetInfo;

	return 0;
}

RulesetInfo * RulesetStore::GetRulesetInfo(int id)
{
	// 先不要去寫資料庫，直接在這邊把遊戲規則寫死就好
	// TODO: 這個是錯誤寫法，以後至少要改成從一個文檔去讀遊戲規則名稱，不能寫在程式裡

	//return rulesetInfos[id];

	switch (id) {
	case 0:
		// no ruleset
		return nullptr;
		break;

	case 1:
		return new RulesetInfo("MeteorRuleset", 1);
		break;

	case 2:
		return new RulesetInfo("InstantRuleset", 2);
		break;

	case 3:
		return new RulesetInfo("RecordRuleset", 3);
		break;
	}
	return nullptr;

	// --------後面不用--------

	DatabaseContext* context = getContext();

	vector<RulesetInfo*>* infos = context->GetRulesetInfos()->GetEntitiesOfPropertiyInt("Id", id);
	if (infos->size() > 0) {
		RulesetInfo* info = infos->at(0);
		delete infos;
		return info;
	}

	delete infos;
	return nullptr;
}
