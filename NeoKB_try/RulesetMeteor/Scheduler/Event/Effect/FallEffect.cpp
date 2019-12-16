#include "FallEffect.h"

#include "../../../Ruleset/Judgements/MeteorHitWindow.h"


using namespace Meteor::Schedulers::Events::Effects;
using namespace std;
using namespace Meteor::Rulesets::Judgements;



FallEffect::FallEffect(): Effect(), HasPitch(Pitch::None)
{
	throw logic_error("FallEffect::FallEffect() : This constructor is only for compile-time assurance. Not available to execute.");
}

FallEffect::FallEffect(Pitch p, int yPos, MTO_FLOAT sTime, MTO_FLOAT l, MTO_FLOAT sp): Effect(int(p), yPos, sTime, l), HasPitch(p)
{
	hitWindow = new MeteorHitWindow();
	SetSpeed(sp);
}

string FallEffect::GetTypeName()
{
	return "FallEffect";
}

Pitch FallEffect::GetPitch()
{
	return pitch;
}
