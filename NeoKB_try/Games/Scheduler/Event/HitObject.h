#ifndef HIT_OBJECT_H
#define HIT_OBJECT_H


#include "EventProcessor.h"
#include "../../Ruleset/Judgements/Judgement.h"
#include "../../Ruleset/Judgements/HitWindow.h"
#include "ArmedState.h"
#include <vector>




using namespace Games::Rulesets::Judgements;
using namespace std;


namespace Games {
namespace Schedulers{
namespace Events{

	class HitObject : virtual public EventProcessor<Event> {

		bool hasJudgementResult = false;

		bool judgementOccurred = false;

		ActionList<int, HitObject*, Judgement*> onJudgement;

		HitWindow* hitWindow;

		ArmedState state = ArmedState::Idle;

		vector<Judgement*> judgements;

		int setState(ArmedState s);

	public:

		template<class _Type>
		int AddOnJudgement(_Type* callableObject, function<int(HitObject*, Judgement*)> callback, string name = "HandleJudgement") {
			

			onJudgement.Add(callableObject, callback, name);

			return 0;
		}

	protected:

		bool UpdateJudgement(bool isUserTriggered);

		virtual int CheckForJudgement(bool isUserTriggered, double offset) = 0;

		int AddJudgement(Judgement* judgement);

	};

}}}






#endif
