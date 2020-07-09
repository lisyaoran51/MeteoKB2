#include "EruptEffect.h"



using namespace Meteor::Schedulers::Events::Effects;



EruptEffect::EruptEffect()
{
	throw logic_error("EruptEffect() : This constructor is only for compile-time assurance. Not available to execute.");
}

EruptEffect::EruptEffect(int xPos, int yPos, MTO_FLOAT sTime, MTO_FLOAT l, MTO_FLOAT sp) : Effect(xPos, yPos, sTime, l)
{
	SetSpeed(sp);
}

int EruptEffect::ApplyDefaultValues(SmDifficulty * smDifficulty)
{
	AdjustSpeed(smDifficulty->FallSpeed);
	return 0;
}

int EruptEffect::SetTargetHeight(int tHeight)
{
	targetHeight = tHeight;
	isSpeedAdjustable = true;
	return 0;
}

int EruptEffect::AdjustSpeed(float sp)
{
	if (!isSpeedAdjustable) {
		// log() << effect start time = ? 
		throw runtime_error(" EruptEffect::AdjustSpeed(): speed is not adjustable");
	}

	// TODO: �o��g�k���F�A����A��A���W���ɭԭn�Ҽ{���٦�target height�H�U���d��A���ӬOfalllength���ӰѼ�

	float newSpeed = sp;
	startTime = startTime + targetHeight / speed - targetHeight / newSpeed;
	lifeTime = lifeTime * speed / newSpeed;
	speed = newSpeed;
	return 0;
}

string EruptEffect::GetTypeName()
{
	return "EruptEffect";
}
