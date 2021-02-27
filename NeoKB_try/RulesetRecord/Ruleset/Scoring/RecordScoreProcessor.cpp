#include "RecordScoreProcessor.h"


using namespace Record::Rulesets::Scoring;


RecordScoreProcessor::RecordScoreProcessor(RulesetExecutor<Event>* rExecutor) : ScoreProcessor(rExecutor) {





}

bool RecordScoreProcessor::getIsCompleted()
{
	return false;
}
