#include "InstantScoreProcessor.h"


using namespace Instant::Rulesets::Scoring;


InstantScoreProcessor::InstantScoreProcessor(RulesetExecutor<Event>* rExecutor) : ScoreProcessor(rExecutor) {





}

bool InstantScoreProcessor::getIsCompleted()
{
	return false;
}
