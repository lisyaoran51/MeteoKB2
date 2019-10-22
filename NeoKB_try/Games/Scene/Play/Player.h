#ifndef PLAYER_H
#define PLAYER_H

#include "../../Ruleset/RulesetExecutor.h"
#include "../../Sheetmusic/SheetmusicManager.h"
#include "../../Sheetmusic/WorkingSheetmusic.h"
#include "../../../Framework/Configurations/FrameworkConfigManager.h"
#include "../MeteoScene.h"
#include "../../../Framework/Timing/DecoupledInterpolatingFramedClock.h"
#include "../../../Framework/Timing/OffsetFramedClock.h"
#include "../../../Framework/Timing/TimeController.h"




using namespace Games::Rulesets;
using namespace Games::Sheetmusics;
using namespace Framework::Configurations;
using namespace Framework::Scenes;
using namespace Games::Scenes;
using namespace Framework::Timing;


namespace Games {
namespace Scenes {
namespace Play {

	class Player: public MeteoScene {

		AdjustableClock* adjustableClock;

		DecoupledInterpolatingFramedClock* decoupledClock;

		OffsetFramedClock* offsetClock;

		TimeController* timeController;

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