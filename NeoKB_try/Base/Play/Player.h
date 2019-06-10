#ifndef PLAYER_H
#define PLAYER_H

#include "../../Util/Update/Updatable.h"
#include "../../Util/Hierachal/ChildAddable.h"
#include "../Ruleset/RulesetExecutor.h"
#include "../Sheetmusic/SheetmusicManager.h"
#include "../Sheetmusic/WorkingSheetmusic.h"
#include "../Config/FrameworkConfigManager.h"



using namespace Util::Hierachal;
using namespace Base::Rulesets;
using namespace Base::Sheetmusics;
using namespace Base::Config;

namespace Base {
namespace Play {

	class Player: public ChildAddable {

		SmManager* smManager;

		WorkingSm* workingSm;

		Ruleset* ruleset;

		RulesetInfo* rulesetInfo;

		RulesetExecutor<Event>* rulesetExecutor;
		
		/// <summary>
		/// load in things like sm, Ruleset info. then create Ruleset executor
		/// </summary>
		int load();
		
		int load(FrameworkConfigManager* f);

	public:

		Player();

		~Player();

	};

}
}








#endif