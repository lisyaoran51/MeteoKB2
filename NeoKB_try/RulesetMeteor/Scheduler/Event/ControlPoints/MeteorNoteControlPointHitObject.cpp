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
