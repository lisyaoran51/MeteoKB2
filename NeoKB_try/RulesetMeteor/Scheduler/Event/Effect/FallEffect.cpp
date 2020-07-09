#include "FallEffect.h"

#include "../../../Ruleset/Judgements/MeteorHitWindow.h"


using namespace Meteor::Schedulers::Events::Effects;
using namespace std;
using namespace Meteor::Rulesets::Judgements;



FallEffect::FallEffect(): Effect()
{
	throw logic_error("FallEffect::FallEffect() : This constructor is only for compile-time assurance. Not available to execute.");
}

FallEffect::FallEffect(int xPos, int yPos, MTO_FLOAT sTime, MTO_FLOAT l, MTO_FLOAT sp): Effect(xPos, yPos, sTime, l)
{
	SetSpeed(sp);
}

int FallEffect::ApplyDefaultValues(SmDifficulty * smDifficulty)
{

	AdjustSpeed(smDifficulty->FallSpeed);

	return 0;
}

int FallEffect::SetTargetHeight(int tHeight)
{
	targetHeight = tHeight;
	isSpeedAdjustable = true;

	return 0;
}

int FallEffect::AdjustSpeed(float sp)
{
	if (!isSpeedAdjustable) {
		// log() << effect start time = ? 
		throw runtime_error(" FallEffect::AdjustSpeed(): speed is not adjustable");
	}

	float newSpeed = sp;

	startTime = startTime + targetHeight / speed - targetHeight / newSpeed;
	lifeTime = lifeTime * speed / newSpeed;
	speed = newSpeed;

	return 0;
}

string FallEffect::GetTypeName()
{
	return "FallEffect";
}
