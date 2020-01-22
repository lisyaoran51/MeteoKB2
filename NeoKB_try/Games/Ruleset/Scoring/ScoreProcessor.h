#ifndef SCORE_PROCESSOR_H
#define SCORE_PROCESSOR_H


#include "../Judgements/Judgement.h"
#include "../../../Util/DataStructure/ActionList.h"


using namespace Games::Rulesets::Judgements;
using namespace Util::DataStructure;



namespace Games {
namespace Rulesets{
namespace Scoring{

	class ScoreProcessor {


	public:

		int AddJudgement(Judgement* judgement);

		int NotifyNewJudgement(Judgement* judgement);

	protected:

		int hits = 0;

		int maxHits = 9999;

		ActionList<int, Judgement*> onNewJudgement;

		ActionList<int> onCompletion;

		bool getIsCompleted();


	};

}
}
}







#endif
