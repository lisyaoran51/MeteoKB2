#ifndef METEOR_RULESET_EXCUTOR_H
#define METEOR_RULESET_EXCUTOR_H

#include "../../Games/Ruleset/Ruleset.h"
#include "../../Framework/Input/PassThroughInputManager.h"
#include "../../Framework/Timing/TimeController.h"
#include "../../Games/Ruleset/Scoring/ScoreProcessor.h"
#include "../Timing/MeteorTimeController.h"


using namespace Games::Rulesets;
using namespace Games::Schedulers::Events;
using namespace Framework::Input;
using namespace Framework::Timing;
using namespace Games::Rulesets::Scoring;
using namespace Meteor::Timing;




namespace Meteor {
namespace Rulesets {

	class MeteorRulesetExecutor: public RulesetExecutor<Event>{

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

		virtual SmConverter* createSmConverter(PatternGenerator* pg);

		virtual SmPostprocessor* createSmPostprocessor();

		int load();

		int load(MeteorTimeController* t);

	public:

		MeteorRulesetExecutor();

		virtual ~MeteorRulesetExecutor() = default;

		virtual int LazyConstruct(WorkingSm* w, Ruleset* r);

		virtual PassThroughInputManager* CreateInputManager();

		virtual TimeController* CreateTimeController();
		
		virtual SpeedAdjuster* CreateSpeedAdjuster();

		virtual ScoreProcessor* CreateScoreProcessor();

		virtual int Elapse(MTO_FLOAT elapsedTime);

	protected:

		Playfield* createPlayfield();

		virtual EventProcessor<Event>* getEventProcessor(Event* e);

		string GetProcessorType(string eventType);

		virtual int playfieldLoad();

	};

}
}







#endif