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
	return score;
}

Bindable<double>* ScoreProcessor::GetAccuracy()
{
	return accuracy;
}

Bindable<int>* ScoreProcessor::GetCombo()
{
	return combo;
}

int ScoreProcessor::PopulateScore(Score * s)
{
	s->hits = hits;
	s->maxHits = maxHits;
	s->score = score->GetValue();
	s->maxScore = maxScore;
	s->combo = highestCombo->GetValue();
	s->accuracy = accuracy->GetValue();

	s->hitResults[HitResult::None] = hitResults[HitResult::None];
	s->hitResults[HitResult::Miss] = hitResults[HitResult::Miss];
	s->hitResults[HitResult::Bad] = hitResults[HitResult::Bad];
	s->hitResults[HitResult::Ok] = hitResults[HitResult::Ok];
	s->hitResults[HitResult::Good] = hitResults[HitResult::Good];
	s->hitResults[HitResult::Great] = hitResults[HitResult::Great];
	s->hitResults[HitResult::Perfect] = hitResults[HitResult::Perfect];

	return 0;
}

ScoreProcessor::ScoreProcessor(RulesetExecutor<Event>* rExecutor)
{

	eventProcessors = rExecutor->GetPlayfield()->GetEventProcessors();

	maxHits = 0;
	maxScore = 0;

	hitResults[HitResult::None] = 0;
	hitResults[HitResult::Miss] = 0;
	hitResults[HitResult::Bad] = 0;
	hitResults[HitResult::Ok] = 0;
	hitResults[HitResult::Good] = 0;
	hitResults[HitResult::Great] = 0;
	hitResults[HitResult::Perfect] = 0;

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
	delete score;
	score = nullptr;
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

	/* 記錄每一種得分的數量 */
	hitResults[judgement->GetResult()] = hitResults[judgement->GetResult()] + 1;


	baseScore += judgement->GetResultScore();
	rollingMaxBaseScore += judgement->GetMaxResultScore();

	score->SetValue(score->GetValue() + judgement->GetResultScore());	// 最後結算用的成績，以後可以在這邊做依些額外權重加乘

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

	score->SetValue(0);
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
