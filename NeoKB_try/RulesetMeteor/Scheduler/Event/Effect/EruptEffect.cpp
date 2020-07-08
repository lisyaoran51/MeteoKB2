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

string EruptEffect::GetTypeName()
{
	return "EruptEffect";
}
