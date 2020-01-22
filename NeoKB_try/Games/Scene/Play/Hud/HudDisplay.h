#ifndef HUD_DISPLAY_H
#define HUD_DISPLAY_H


#include "../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../Ruleset/Scoring/ScoreProcessor.h"
#include "../../../Ruleset/RulesetExecutor.h"
#include "../../../../Framework/Timing/TimeController.h"
#include "../../../Scheduler/Event/Event.h"


using namespace Framework::Allocation::Hierachal;
using namespace Games::Rulesets::Scoring;
using namespace Games::Rulesets;
using namespace Framework::Timing;
using namespace Games::Schedulers::Events;


namespace Games {
namespace Scenes {
namespace Play {
namespace Hud {

	class HudDisplay : public Container {

	public:


		int BindScoreProcessor(ScoreProcessor* sProcessor);

		int BindRulesetExecutor(RulesetExecutor<Event>* rExecutor);

		int BindTimeController(TimeController* tController);

	};

}}}}



#endif