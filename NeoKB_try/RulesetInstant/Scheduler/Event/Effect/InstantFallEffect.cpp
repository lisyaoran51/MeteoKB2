#include "InstantFallEffect.h"




using namespace Instant::Schedulers::Events::Effects;
using namespace std;



InstantFallEffect::InstantFallEffect(): Effect()
{
	throw logic_error("InstantFallEffect::InstantFallEffect() : This constructor is only for compile-time assurance. Not available to execute.");
}

InstantFallEffect::InstantFallEffect(int xPos, int yPos, MTO_FLOAT sTime, MTO_FLOAT l, MTO_FLOAT sp): Effect(xPos, yPos, sTime, l)
{
	effectPinType = EffectPinType::ByPosition;
	SetSpeed(sp);
	lifeType = EventLifeType::Timed;
}

InstantFallEffect::InstantFallEffect(Pitch p, MTO_FLOAT sTime, MTO_FLOAT l, MTO_FLOAT sp) : Effect(0, 0, sTime, l)
{
	effectPinType = EffectPinType::ByPitch;
	pitch = p;
	SetSpeed(sp);
}

int InstantFallEffect::GetX()
{
	if (effectPinType == EffectPinType::ByPosition)
		return Effect::GetX();
	if (effectPinType == EffectPinType::ByPitch) {
		return int(pitch);
	}

	LOG(LogLevel::Warning) << "InstantFallEffect::GetX() : no effect pin type set.";

	return 0;
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

EffectPinType InstantFallEffect::GetEffectPinType()
{
	return effectPinType;
}

Pitch InstantFallEffect::GetPitch()
{
	if (effectPinType == EffectPinType::ByPitch)
		return pitch;
	else
		return Pitch::None;
}

string InstantFallEffect::GetTypeName()
{
	return "InstantFallEffect";
}

Effect * InstantFallEffect::Clone()
{
	if (effectPinType == EffectPinType::ByPitch)
		return new InstantFallEffect(pitch, startTime, lifeTime, speed);
	if (effectPinType == EffectPinType::ByPosition)
		return new InstantFallEffect(x, y, startTime, lifeTime, speed);

	return nullptr;
}
