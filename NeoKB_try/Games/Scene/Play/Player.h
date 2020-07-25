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
#include "../../../Instruments/Instrument.h"
#include "Hud/HudDisplay.h"
#include "../../Configurations/MeteoConfigManager.h"




using namespace Games::Rulesets;
using namespace Games::Sheetmusics;
using namespace Framework::Configurations;
using namespace Framework::Scenes;
using namespace Games::Scenes;
using namespace Framework::Timing;
using namespace Instruments;
using namespace Games::Scenes::Play::Hud;
using namespace Games::Configurations;



namespace Games {
namespace Scenes {
namespace Play {

	class Player: public MeteoScene {

		AdjustableClock* adjustableClock = nullptr;

		DecoupledInterpolatingFramedClock* decoupledClock = nullptr;

		OffsetFramedClock* offsetClock = nullptr;

		TimeController* timeController = nullptr;

		SpeedAdjuster* speedAdjuster = nullptr;

		SmManager* smManager = nullptr;

		Instrument* instrument = nullptr;


		Ruleset* ruleset = nullptr;


		RulesetExecutor<Event>* rulesetExecutor = nullptr;

		vector<Modifier*>* modifiers = nullptr;

		ScoreProcessor* scoreProcessor = nullptr;
		

		HudDisplay* hudDisplay = nullptr;

		/// <summary>
		/// load in things like sm, Ruleset info. then create Ruleset executor
		/// </summary>
		int load();
		
		int load(MeteoConfigManager* m, Instrument* i);

	public:

		Player();

		~Player();

		int Restart();

		template<class _Type>
		int AddOnRestartRequested(_Type* callableObject, function<int(void)> callback, string name = "HandleRestartRequest") {
			onRestartRequested.Add(callableObject, callback, name);
			return 0;
		}

	protected:

		ActionList<int(void)> onRestartRequested;

		float audioOffset = 0;

		int onCompletion();

	};

}
}}








#endif