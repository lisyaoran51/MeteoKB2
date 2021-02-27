#ifndef RECORD_SCORE_PROCESSOR_H
#define RECORD_SCORE_PROCESSOR_H


#include "../../../Games/Ruleset/Scoring/ScoreProcessor.h"


using namespace Games::Rulesets::Scoring;


namespace Record {
namespace Rulesets{
namespace Scoring{

	class RecordScoreProcessor : public ScoreProcessor {

	public:

		RecordScoreProcessor(RulesetExecutor<Event>* rExecutor);

	protected:

		virtual bool getIsCompleted();

	};

}}}






#endif