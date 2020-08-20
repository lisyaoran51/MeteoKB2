#include "MeteorInputKeyControlPointHitObject.h"

#include "../../../Ruleset/Judgements/MeteorHitWindow.h"
#include "../../../Ruleset/Judgements/MeteorJudgement.h"



using namespace Meteor::Schedulers::Events::ControlPoints;
using namespace Meteor::Rulesets::Judgements;
using namespace Meteor::Schedulers::Events::ControlPoints;

MeteorInputKeyControlPointHitObject::MeteorInputKeyControlPointHitObject(): InputKeyControlPointHitObject(new MeteorHitWindow())
{
}

Judgement * MeteorInputKeyControlPointHitObject::createJudgement(HitResult hResult)
{
	return new MeteorJudgement(hResult);
}

Judgement * MeteorInputKeyControlPointHitObject::GetBestJudgement()
{
	return new MeteorJudgement(HitResult::Perfect);
}

HitWindow * MeteorInputKeyControlPointHitObject::createHitWindow(SmDifficulty * smDifficulty)
{
	return new MeteorHitWindow();
}
