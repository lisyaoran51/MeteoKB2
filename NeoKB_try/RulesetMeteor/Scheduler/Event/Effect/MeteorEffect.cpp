#include "MeteorEffect.h"


using namespace Meteor::Schedulers::Events::Effects;


MeteorEffect::MeteorEffect()
{
	throw logic_error("MeteorEffect::MeteorEffect() : This constructor is only for compile-time assurance. Not available to execute.");
}

MeteorEffect::MeteorEffect(int xPos, int yPos, MTO_FLOAT sTime, MTO_FLOAT l): Effect(xPos, yPos, sTime, l)
{
}

int MeteorEffect::SetMeteorEffectShiftType(MeteorEffectShiftType mEffectShiftType)
{
	meteorEffectShiftType = mEffectShiftType;

	return 0;
}
