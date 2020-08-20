#include "MeteorNoteControlPointHitObject.h"

#include "../../../Ruleset/Judgements/MeteorHitWindow.h"
#include "../../../Ruleset/Judgements/MeteorJudgement.h"


using namespace Meteor::Schedulers::Events::ControlPoints;
using namespace Meteor::Rulesets::Judgements;



MeteorNoteControlPointHitObject::MeteorNoteControlPointHitObject(): NoteControlPointHitObject(new MeteorHitWindow())
{
}

Judgement * MeteorNoteControlPointHitObject::createJudgement(HitResult hResult)
{
	return new MeteorJudgement(hResult);
}

Judgement * MeteorNoteControlPointHitObject::GetBestJudgement()
{
	return new MeteorJudgement(HitResult::Perfect);
}

HitWindow * MeteorNoteControlPointHitObject::createHitWindow(SmDifficulty * smDifficulty)
{
	return new MeteorHitWindow();
}
