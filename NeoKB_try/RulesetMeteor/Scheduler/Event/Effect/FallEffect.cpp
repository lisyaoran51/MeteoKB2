#include "FallEffect.h"

using namespace Meteor::Schedulers::Events::Effects;
using namespace std;

FallEffect::FallEffect(): Effect()
{
	throw logic_error("FallEffect::FallEffect() : This constructor is only for compile-time assurance. Not available to execute.");
}

FallEffect::FallEffect(Pitch p, int yPos, MTO_FLOAT sTime, MTO_FLOAT l, MTO_FLOAT sp): Effect(int(p), yPos, sTime, l)
{
	pitch = p;
	SetSpeed(sp);
}

string FallEffect::GetTypeName()
{
	return "FallEffect";
}

Pitch FallEffect::GetPitch()
{
	return pitch;
}
