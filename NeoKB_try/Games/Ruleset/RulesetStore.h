#ifndef RULESET_STORE_H
#define RULESET_STORE_H

#include "../../Framework/Database/DatabaseStore.h"


using namespace Framework::Database;


namespace Games {
namespace Rulesets{

	/// <summary>
	/// �bosu�O�C���}�C���ɡA�A���sŪ�ɮסA�M��Ū�n�g�JDB���C�ڭ̤���o�˼g�A�n�令�̲v�qDBŪ���
	/// </summary>
	class RulesetStore : public DatabaseStore {

		/// <summary>
		/// �o�ӭnload�ɮ׶idatabase�A�����n��
		/// </summary>
		int loadRulesetFromFile(string path);

		map<int, RulesetInfo*> rulesetInfos;

	public:

		RulesetStore(function<DatabaseContext*(void)> getContext, Storage* s = nullptr);

		/// <summary>
		/// �Χ��O�o�n�R���A���M�|memory leak
		/// </summary>
		vector<RulesetInfo*>* GetRulesetInfos();

		int SetRulesetInfo(int id, RulesetInfo* rulesetInfo);

		RulesetInfo* GetRulesetInfo(int id);

	};

}}


#endif