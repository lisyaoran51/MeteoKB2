#ifndef SCORE_PROCESSOR_H
#define SCORE_PROCESSOR_H


#include "../Judgements/Judgement.h"


using namespace Games::Rulesets::Judgements;


namespace Games {
namespace Rulesets{
namespace Scoring{

	class ScoreProcessor {


	public:

		int AddJudgement(Judgement* judgement);

	};

}
}
}







#endif
