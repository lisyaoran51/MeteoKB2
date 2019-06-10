#ifndef METEOR_RULESET_EXCUTOR_H
#define METEOR_RULESET_EXCUTOR_H

#include "../../Base/Ruleset/Ruleset.h"


using namespace Base::Rulesets;
using namespace Base::Schedulers::Events;

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

	public:

		MeteorRulesetExecutor();

		virtual ~MeteorRulesetExecutor() = default;

		virtual int LazyConstruct(WorkingSm* w);

		virtual int Elapse(MTO_FLOAT elapsedTime);

	protected:

		Playfield* createPlayfield();

		virtual EventProcessor<Event>* getEventProcessor(Event* e);

		string GetProcessorType(string eventType);

	};

}
}







#endif