#include "TargetLineEffect.h"


using namespace Meteor::Schedulers::Events::Effects;


TargetLineEffect::TargetLineEffect() : Effect()
{
	throw logic_error("TargetLineEffect::TargetLineEffect() : This constructor is only for compile-time assurance. Not available to execute.");
}

TargetLineEffect::TargetLineEffect(int wHeight, int bHeight, MTO_FLOAT sTime, MTO_FLOAT l, MTO_FLOAT bSpeed) : Effect(0, 0, sTime, l)
{

	whiteKeyTargetHeight = wHeight;
	blackKeyTargetHeight = bHeight;
	SetSpeed(bSpeed);

}

string TargetLineEffect::GetTypeName()
{
	return "TargetLineEffect";
}
