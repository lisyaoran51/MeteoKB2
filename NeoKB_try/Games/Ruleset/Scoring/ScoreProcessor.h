#ifndef SCORE_PROCESSOR_H
#define SCORE_PROCESSOR_H


#include "../Judgements/Judgement.h"
#include "../../../Util/DataStructure/ActionList.h"
#include "../../Scheduler/Event/Event.h"
#include "../../Scheduler/Event/EventProcessor.h"
#include "../../../Util/DataStructure//Bindable.h"


using namespace Games::Rulesets::Judgements;
using namespace Util::DataStructure;
using namespace Games::Schedulers::Events;


namespace Games {
namespace Rulesets {

	template<class T>
	class RulesetExecutor;

}}
using namespace Games::Rulesets;


namespace Games {
namespace Rulesets {
namespace Scoring {

	class ScoreProcessor {


	public:


		template<class _Type>
		int AddOnAllJudged(_Type* callableObject, function<int(void)> callback, string name = "HandleCompletion") {
			onAllJudged.Add(callableObject, callback, name);
		}

		template<class _Type>
		int AddOnNewJudgement(_Type* callableObject, function<int(Judgement*)> callback, string name = "HandleNewJudgement") {
			onNewJudgement.Add(callableObject, callback, name);
		}

		Bindable<double>* GetTotalScore();

		Bindable<double>* GetAccuracy();

		Bindable<int>* GetCombo();

	protected:

		ScoreProcessor(RulesetExecutor<Event>* rExecutor);

		vector<EventProcessor<Event>*>* eventProcessors = nullptr;

		int hits = 0;

		int maxHits = 9999;

		double baseScore = 0;

		double rollingMaxBaseScore = 0;

		double maxScore = 1.0;

		Bindable<double>* totalScore = new Bindable<double>(0);

		Bindable<double>* accuracy = new Bindable<double>(1);

		Bindable<int>* combo = new Bindable<int>(0);

		ActionList<int, Judgement*> onNewJudgement;

		ActionList<int> onAllJudged;

		bool getIsCompleted();

		int addJudgement(Judgement* judgement);

		int addUpJudgementScore(Judgement* judgement);

		int NotifyNewJudgement(Judgement* judgement);

		int reset(bool storeResults);

	};

}
}
}







#endif
