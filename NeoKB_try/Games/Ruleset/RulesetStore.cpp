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
	// �����n�h�g��Ʈw�A�����b�o���C���W�h�g���N�n
	// TODO: �o�ӬO���~�g�k�A�H��ܤ֭n�令�q�@�Ӥ��ɥhŪ�C���W�h�W�١A����g�b�{����

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

	// --------�᭱����--------

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
