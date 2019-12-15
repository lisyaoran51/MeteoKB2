#include "MeteorJudgement.h"


using namespace Meteor::Rulesets::Judgements;

MeteorJudgement::MeteorJudgement(HitResult r) : Judgement(r)
{
}

double MeteorJudgement::getResultNumber(HitResult hResult)
{

	switch (hResult)
	{
	case HitResult::Meh:
		return 50;
	case HitResult::Ok:
		return 100;
	case HitResult::Good:
		return 200;
	case HitResult::Great:
	case HitResult::Perfect:
		return 300;
	default:
		return 0;
	}

	return 0.0;
}
