#ifndef INSTANT_RULESET_EXCUTOR_H
#define INSTANT_RULESET_EXCUTOR_H

#include "../../Games/Ruleset/Ruleset.h"
#include "../../Framework/Input/PassThroughInputManager.h"
#include "../../Framework/Timing/TimeController.h"
#include "../../Games/Ruleset/Scoring/ScoreProcessor.h"
#include "../../Instruments/CompositeMeteoPiano.h"


using namespace Games::Rulesets;
using namespace Games::Schedulers::Events;
using namespace Framework::Input;
using namespace Framework::Timing;
using namespace Games::Rulesets::Scoring;
using namespace Instruments;




namespace Instant {
namespace Rulesets {

	class InstantRulesetExecutor: public RulesetExecutor<Event>{

		/// <summary>
		/// 判斷這個物建有沒有被建構，建構後才可使用
		///	</summary>
		bool constructed;

		///<summary>
		/// 這個試做什麼用的
		/// 在get event processor時，把這個event要用到的effect mapper擺進去???
		/// **用這個不太好，效能會很差，應該直接用判斷式，不要查table
		/// **查table的好處是可以把executor和event processor分開，降低偶合
		///</summary>
		map<string, string> eventProcessorTable;

		virtual SmConverter* createSmConverter(PatternGenerator* pg) override;

		virtual SmPostprocessor* createSmPostprocessor() override;

		int load();

		int load(Instrument* i);

	public:

		InstantRulesetExecutor();

		virtual ~InstantRulesetExecutor();

		virtual int LazyConstruct(WorkingSm* w, Ruleset* r);

		virtual PassThroughInputManager* CreateInputManager() override;

		virtual TimeController* CreateTimeController() override;
		
		virtual SpeedAdjuster* CreateSpeedAdjuster() override;

		virtual ScoreProcessor* CreateScoreProcessor() override;

		virtual ReplayRecorder* CreateReplayRecorder() override;

		virtual Result* CreateResult(Score* s) override;

		virtual int Elapse(MTO_FLOAT elapsedTime);

	protected:

		CompositeMeteoPiano* compositeMeteoPiano = nullptr;

		Playfield* createPlayfield();

		virtual EventProcessor<Event>* getEventProcessor(Event* e);

		string GetProcessorType(string eventType);

		virtual int playfieldLoad() override;

	};

}
}







#endif