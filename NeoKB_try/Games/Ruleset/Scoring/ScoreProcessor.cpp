#include "ScoreProcessor.h"

#include "../RulesetExecutor.h"


using namespace Games::Rulesets::Scoring;
using namespace Games::Rulesets;


Bindable<double>* ScoreProcessor::GetTotalScore()
{
	return totalScore;
}

Bindable<double>* ScoreProcessor::GetAccuracy()
{
	return accuracy;
}

Bindable<int>* ScoreProcessor::GetCombo()
{
	return combo;
}

ScoreProcessor::ScoreProcessor(RulesetExecutor<Event>* rExecutor)
{

	eventProcessors = rExecutor->GetPlayfield()->GetEventProcessors();

	maxHits = 0;
	maxScore = 0;

	for (int i = 0; i < eventProcessors->size(); i++) {
		if (dynamic_cast<HitObject*>(eventProcessors->at(i))) {

			maxHits++;

			Judgement* judgement = dynamic_cast<HitObject*>(eventProcessors->at(i))->GetBestJudgement();

			maxScore += judgement->GetMaxResultScore();

			delete judgement;
		}
	}

	rExecutor->AddOnJudgement(this, bind(&ScoreProcessor::addJudgement, this, placeholders::_1), "ScoreProcessor::addJudgement");

	reset(true);

}

bool ScoreProcessor::getIsCompleted()
{
	return hits == maxHits;
}

int ScoreProcessor::addJudgement(Judgement * judgement)
{

	addUpJudgementScore(judgement);
	NotifyNewJudgement(judgement);

	return 0;
}

int ScoreProcessor::addUpJudgementScore(Judgement * judgement)
{
	double bonusScore = 0;

	switch (judgement->GetResult())
	{
	case HitResult::None:
		break;
	case HitResult::Miss:
		combo->SetValue(0);
		break;
	default:
		combo->SetValue(combo->GetValue() + 1);
		break;
	}

	baseScore += judgement->GetResultScore();
	rollingMaxBaseScore += judgement->GetMaxResultScore();

	hits++;
	

	if (rollingMaxBaseScore != 0)
		accuracy->SetValue(baseScore / rollingMaxBaseScore);

	/*
	switch (Mode.Value)
	{
	case ScoringMode.Standardised:
		TotalScore.Value = max_score * (base_portion * baseScore / maxBaseScore + combo_portion * HighestCombo / maxHighestCombo) + bonusScore;
		break;
	case ScoringMode.Exponential:
		TotalScore.Value = (baseScore + bonusScore) * Math.Log(HighestCombo + 1, 2);
		break;
	}
	*/
}

int ScoreProcessor::NotifyNewJudgement(Judgement * judgement)
{
	onNewJudgement.Trigger(judgement);

	if (getIsCompleted()) {
		onAllJudged.Trigger();
	}

	return 0;
}

int ScoreProcessor::reset(bool storeResults)
{

	totalScore->SetValue(0);
	accuracy->SetValue(1);
	combo->SetValue(0);
	//Rank.Value = ScoreRank.X;
	//HighestCombo.Value = 0;

	hits = 0;
	baseScore = 0;
	rollingMaxBaseScore = 0;

	return 0;
}
