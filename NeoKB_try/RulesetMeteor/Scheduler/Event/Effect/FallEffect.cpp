#include "FallEffect.h"

#include "../../../Ruleset/Judgements/MeteorHitWindow.h"


using namespace Meteor::Schedulers::Events::Effects;
using namespace std;
using namespace Meteor::Rulesets::Judgements;



FallEffect::FallEffect(): MeteorEffect()
{
	throw logic_error("FallEffect::FallEffect() : This constructor is only for compile-time assurance. Not available to execute.");
}

FallEffect::FallEffect(int xPos, int yPos, MTO_FLOAT sTime, MTO_FLOAT l, MTO_FLOAT sp): MeteorEffect(xPos, yPos, sTime, l)
{
	SetSpeed(sp);
}

int FallEffect::ApplyDefaultValues(SmDifficulty * smDifficulty)
{

	AdjustSpeed(smDifficulty->Speed);

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

Effect * FallEffect::Clone()
{
	FallEffect* newEvent = new FallEffect(x, y, startTime, lifeTime, speed);
	newEvent->SetTargetHeight(targetHeight);
	newEvent->SetMeteorEffectShiftType(meteorEffectShiftType);
	newEvent->SetSourceEvent(sourceEvent);
	return newEvent;
}

int FallEffect::GetX()
{
	switch (meteorEffectShiftType) {
	case MeteorEffectShiftType::None:
		return x;
		break;

	case MeteorEffectShiftType::LoweredOctave:
		return x + 12;
		break;

	case MeteorEffectShiftType::RaisedOctave:
		return x - 12;
		break;
	}


	return x;
}
