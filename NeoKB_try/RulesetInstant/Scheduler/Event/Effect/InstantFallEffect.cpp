#include "InstantFallEffect.h"




using namespace Instant::Schedulers::Events::Effects;
using namespace std;



InstantFallEffect::InstantFallEffect(): Effect()
{
	throw logic_error("InstantFallEffect::InstantFallEffect() : This constructor is only for compile-time assurance. Not available to execute.");
}

InstantFallEffect::InstantFallEffect(int xPos, int yPos, MTO_FLOAT sTime, MTO_FLOAT l, MTO_FLOAT sp): Effect(xPos, yPos, sTime, l)
{
	SetSpeed(sp);
}

int InstantFallEffect::ApplyDefaultValues(SmDifficulty * smDifficulty)
{

	AdjustSpeed(smDifficulty->Speed);

	return 0;
}

int InstantFallEffect::SetTargetHeight(int tHeight)
{
	targetHeight = tHeight;
	isSpeedAdjustable = true;

	return 0;
}

int InstantFallEffect::AdjustSpeed(float sp)
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

string InstantFallEffect::GetTypeName()
{
	return "InstantFallEffect";
}

Effect * InstantFallEffect::Clone()
{
	return new InstantFallEffect(x, y, startTime, lifeTime, speed);
}
