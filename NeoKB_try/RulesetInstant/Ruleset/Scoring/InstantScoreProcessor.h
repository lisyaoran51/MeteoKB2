#ifndef METEOR_SCORE_PROCESSOR_H
#define METEOR_SCORE_PROCESSOR_H


#include "../../../Games/Ruleset/Scoring/ScoreProcessor.h"


using namespace Games::Rulesets::Scoring;


namespace Meteor {
namespace Rulesets{
namespace Scoring{

	class MeteorScoreProcessor : public ScoreProcessor {

	public:

		MeteorScoreProcessor(RulesetExecutor<Event>* rExecutor);

	};

}}}






#endif