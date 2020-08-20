#ifndef INSTANT_SCORE_PROCESSOR_H
#define INSTANT_SCORE_PROCESSOR_H


#include "../../../Games/Ruleset/Scoring/ScoreProcessor.h"


using namespace Games::Rulesets::Scoring;


namespace Instant {
namespace Rulesets{
namespace Scoring{

	class InstantScoreProcessor : public ScoreProcessor {

	public:

		InstantScoreProcessor(RulesetExecutor<Event>* rExecutor);

	};

}}}






#endif