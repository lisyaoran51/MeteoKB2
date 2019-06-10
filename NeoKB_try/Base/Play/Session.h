#ifndef SESSION_H
#define SESSION_H

#include <vector>
#include "../../Util/Hierachal/ChildAddable.h"
#include "../Ruleset/RulesetInfo.h"
#include "../Sheetmusic/WorkingSheetmusic.h"
#include "../Sheetmusic/SheetmusicManager.h"

using namespace std;
using namespace Util::Hierachal;
using namespace Base::Sheetmusics;


namespace Base {
namespace Play {

	class Session : public ChildAddable {

		//int load();
		
		//int load(RulesetInfo* r, SmManager* s);

	public:

		Session(RulesetInfo* r, SmManager* s);

		RulesetInfo* GetRulesetInfo();

		WorkingSm* GetWorkingSm();
		
		WorkingSm* GetWorkingSm(string title);

	protected:

		RulesetInfo* rulesetInfo;

		SmManager* smManager;

		SmInfo* smInfo; 	// 這行之後要刪掉
		
		vector<SmInfo*>* smInfos;

	};

}}





#endif