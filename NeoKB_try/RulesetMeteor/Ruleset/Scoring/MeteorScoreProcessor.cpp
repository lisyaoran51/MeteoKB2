#include "MeteorScoreProcessor.h"

#include "../../Scheduler/Event/ControlPoints/MeteorInputKeyControlPointHitObject.h"

using namespace Meteor::Rulesets::Scoring;
using namespace Meteor::Schedulers::Events::ControlPoints;


MeteorScoreProcessor::MeteorScoreProcessor(RulesetExecutor<Event>* rExecutor) : ScoreProcessor(rExecutor) {


	// 如果有踏板，而且是電腦控制踏板的話，要把踏板的打擊點拿掉
	// 這邊先暫時把踏板扣掉，以後再把code改正長一點
	// pedal事件先全部跳過，以後要檢查視不適game control pedal，是的話再跳過，game control pedal現在存在piano和meteo piano裡
	for (int i = 0; i < eventProcessors->size(); i++) {
		if (dynamic_cast<MeteorInputKeyControlPointHitObject*>(eventProcessors->at(i))) {

			maxHits--;

			Judgement* judgement = dynamic_cast<MeteorInputKeyControlPointHitObject*>(eventProcessors->at(i))->GetBestJudgement();

			maxScore -= judgement->GetMaxResultScore();

			delete judgement;
		}
	}



}