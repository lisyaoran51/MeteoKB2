#include "ScoreProcessor.h"

#include "../RulesetExecutor.h"
#include "../../../Util/Log.h"
#include <iomanip>


using namespace Games::Rulesets::Scoring;
using namespace Games::Rulesets;
using namespace Util;
using namespace std;


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

int ScoreProcessor::PopulateScore(Score * score)
{
	score->hits = hits;
	score->maxHits = maxHits;
	score->totalScore = totalScore->GetValue();
	score->combo = combo->GetValue();
	score->highestCombo = highestCombo->GetValue();
	score->accuracy = accuracy->GetValue();


	return 0;
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

ScoreProcessor::~ScoreProcessor()
{
	delete totalScore;
	totalScore = nullptr;
	delete accuracy;
	accuracy = nullptr;
	delete combo;
	combo = nullptr;
	delete highestCombo;
	highestCombo = nullptr;
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
		miss++;
		break;
	default:
		combo->SetValue(combo->GetValue() + 1);
		if (combo->GetValue() > highestCombo->GetValue()) {
			highestCombo->SetValue(combo->GetValue());
		}
		break;
	}

	baseScore += judgement->GetResultScore();
	rollingMaxBaseScore += judgement->GetMaxResultScore();

	hits++;
	
	LOG(LogLevel::Depricated) << "ScoreProcessor::addUpJudgementScore : add score [" << judgement->GetResultScore() << "], total score [" 
		<< baseScore << "], song max score [" << maxScore << "].";

	LOG(LogLevel::Info) << "ScoreProcessor::addUpJudgementScore : add score [" << judgement->GetResultScore() << "], score ["
		<< baseScore << "/" << maxScore << "], hits [" << hits - miss << "/ " << maxHits << "], combo [" 
		<< combo->GetValue() <<"/" << highestCombo->GetValue() << "], [" << fixed << setprecision(1) << (float)baseScore / (float)maxScore * 100.f << "%], progress ["
		<< hits << "/" << maxHits << "].";

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
	highestCombo->SetValue(0);
	//Rank.Value = ScoreRank.X;


	hits = 0;
	miss = 0;
	baseScore = 0;
	rollingMaxBaseScore = 0;

	return 0;
}
