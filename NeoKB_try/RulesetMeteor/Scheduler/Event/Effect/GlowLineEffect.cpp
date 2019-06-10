#include "GlowLineEffect.h"

using namespace Meteor::Schedulers::Events::Effects;
using namespace std;


GlowLineEffect::GlowLineEffect(): Effect()
{
	throw logic_error("ExplodeEffect::ExplodeEffect() : This constructor is only for compile-time assurance. Not available to execute.");
}

GlowLineEffect::GlowLineEffect(int xPos, int yPos, MTO_FLOAT sTime, MTO_FLOAT l, MTO_FLOAT sp): Effect(xPos, yPos, sTime, l)
{
	SetSpeed(sp);
}

string GlowLineEffect::GetTypeName()
{
	return "GlowLineEffect";
}
