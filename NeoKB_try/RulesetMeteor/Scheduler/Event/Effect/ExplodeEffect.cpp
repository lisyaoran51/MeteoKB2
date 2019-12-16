#include "ExplodeEffect.h"

using namespace Meteor::Schedulers::Events::Effects;
using namespace std;

ExplodeEffect::ExplodeEffect(): Effect()
{
	throw logic_error("ExplodeEffect::ExplodeEffect() : This constructor is only for compile-time assurance. Not available to execute.");
}

ExplodeEffect::ExplodeEffect(Pitch p, int yPos, MTO_FLOAT sTime, MTO_FLOAT l): Effect(int(p), yPos, sTime, l)
{
	pitch = p;
	lifeType = EventLifeType::Timed;
}

Pitch ExplodeEffect::GetPitch() {
	return pitch;
}

string ExplodeEffect::GetTypeName()
{
	return "ExplodeEffect";
}
