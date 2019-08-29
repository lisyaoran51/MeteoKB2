#ifndef PLAYER_H
#define PLAYER_H

#include "../../Ruleset/RulesetExecutor.h"
#include "../../Sheetmusic/SheetmusicManager.h"
#include "../../Sheetmusic/WorkingSheetmusic.h"
#include "../../../Framework/Configurations/FrameworkConfigManager.h"
#include "../MeteoScene.h"



using namespace Base::Rulesets;
using namespace Base::Sheetmusics;
using namespace Framework::Configurations;
using namespace Framework::Scenes;
using namespace Base::Scenes;


namespace Base {
namespace Scenes {
namespace Play {

	class Player: public MeteoScene {

		SmManager* smManager;


		Ruleset* ruleset;


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
}}








#endif