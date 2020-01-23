#ifndef HUD_DISPLAY_H
#define HUD_DISPLAY_H


#include "../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../Ruleset/Scoring/ScoreProcessor.h"
#include "../../../Ruleset/RulesetExecutor.h"
#include "../../../../Framework/Timing/TimeController.h"
#include "../../../Scheduler/Event/Event.h"
#include "JudgementFlasher.h"
#include "SongProgress.h"
#include "RollingCounter.h"


using namespace Framework::Allocation::Hierachal;
using namespace Games::Rulesets::Scoring;
using namespace Games::Rulesets;
using namespace Framework::Timing;
using namespace Games::Schedulers::Events;
using namespace Games::Scenes::Play::Hud;


namespace Games {
namespace Scenes {
namespace Play {
namespace Hud {

	class HudDisplay : public Container {

	public:

		HudDisplay();

		int BindScoreProcessor(ScoreProcessor* sProcessor);

		int BindRulesetExecutor(RulesetExecutor<Event>* rExecutor);

		int BindTimeController(TimeController* tController);

	protected:

		RollingCounter<double>* scoreCounter = nullptr;

		RollingCounter<double>* accuracyCounter = nullptr;	// 可能會需要多寫一個persentage counter

		RollingCounter<int>* comboCounter = nullptr;

		SongProgress* songProgress = nullptr;

		JudgementFlasher* judgementFlasher = nullptr;

	};

}}}}



#endif