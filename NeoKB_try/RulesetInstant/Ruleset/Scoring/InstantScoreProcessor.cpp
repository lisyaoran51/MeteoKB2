#include "MeteorScoreProcessor.h"

#include "../../Scheduler/Event/ControlPoints/MeteorInputKeyControlPointHitObject.h"

using namespace Meteor::Rulesets::Scoring;
using namespace Meteor::Schedulers::Events::ControlPoints;


MeteorScoreProcessor::MeteorScoreProcessor(RulesetExecutor<Event>* rExecutor) : ScoreProcessor(rExecutor) {


	// �p�G����O�A�ӥB�O�q�������O���ܡA�n���O�������I����
	// �o����Ȯɧ��O�����A�H��A��code�勵���@�I
	// pedal�ƥ���������L�A�H��n�ˬd�����Agame control pedal�A�O���ܦA���L�Agame control pedal�{�b�s�bpiano�Mmeteo piano��
	for (int i = 0; i < eventProcessors->size(); i++) {
		if (dynamic_cast<MeteorInputKeyControlPointHitObject*>(eventProcessors->at(i))) {

			maxHits--;

			Judgement* judgement = dynamic_cast<MeteorInputKeyControlPointHitObject*>(eventProcessors->at(i))->GetBestJudgement();

			maxScore -= judgement->GetMaxResultScore();

			delete judgement;
		}
	}



}