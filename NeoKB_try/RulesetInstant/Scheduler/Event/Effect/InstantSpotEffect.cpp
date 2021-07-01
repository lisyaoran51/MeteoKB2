#include "InstantSpotEffect.h"


using namespace Instant::Schedulers::Events::Effects;



InstantSpotEffect::InstantSpotEffect()
{
	throw logic_error("InstantSpotEffect::InstantSpotEffect() : This constructor is only for compile-time assurance. Not available to execute.");
}

InstantSpotEffect::InstantSpotEffect(int xPos, int yPos, MTO_FLOAT sTime, MTO_FLOAT l) : Effect(xPos, yPos, sTime, l)
{
	effectPinType = EffectPinType::ByPosition;
}

InstantSpotEffect::InstantSpotEffect(Pitch p, MTO_FLOAT sTime, MTO_FLOAT l) : Effect(0, 0, sTime, l)
{
	effectPinType = EffectPinType::ByPitch;
	pitch = p;
}

EffectPinType InstantSpotEffect::GetEffectPinType()
{
	return effectPinType;
}

Pitch InstantSpotEffect::GetPitch()
{
	if (effectPinType == EffectPinType::ByPitch)
		return pitch;
	else
		return Pitch::None;
}

string InstantSpotEffect::GetTypeName()
{
	return "InstantSpotEffect";
}

Effect * InstantSpotEffect::Clone()
{
	if(effectPinType == EffectPinType::ByPitch)
		return new InstantSpotEffect(pitch, startTime, lifeTime);
	if(effectPinType == EffectPinType::ByPosition)
		return new InstantSpotEffect(x, y, startTime, lifeTime);

	return nullptr;
}
