#ifndef RULESET_STORE_H
#define RULESET_STORE_H

#include "../../Framework/Database/DatabaseStore.h"


using namespace Framework::Database;


namespace Games {
namespace Rulesets{

	/// <summary>
	/// 在osu是每次開遊戲時，再重新讀檔案，然後讀好寫入DB李。我們不能這樣寫，要改成依率從DB讀資料
	/// </summary>
	class RulesetStore : public DatabaseStore {

		/// <summary>
		/// 這個要load檔案進database，先不要用
		/// </summary>
		int loadRulesetFromFile(string path);

		map<int, RulesetInfo*> rulesetInfos;

	public:

		RulesetStore(function<DatabaseContext*(void)> getContext, Storage* s = nullptr);

		/// <summary>
		/// 用完記得要刪掉，不然會memory leak
		/// </summary>
		vector<RulesetInfo*>* GetRulesetInfos();

		int SetRulesetInfo(int id, RulesetInfo* rulesetInfo);

		RulesetInfo* GetRulesetInfo(int id);

	};

}}


#endif