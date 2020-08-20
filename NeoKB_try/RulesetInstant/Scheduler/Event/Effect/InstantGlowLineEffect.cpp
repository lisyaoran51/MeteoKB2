#include "InstantGlowLineEffect.h"

using namespace Instant::Schedulers::Events::Effects;
using namespace std;


InstantGlowLineEffect::InstantGlowLineEffect(): Effect()
{
	throw logic_error("InstantGlowLineEffect::ExplodeEffect() : This constructor is only for compile-time assurance. Not available to execute.");
}

InstantGlowLineEffect::InstantGlowLineEffect(int xPos, int yPos, MTO_FLOAT sTime, MTO_FLOAT l): Effect(xPos, yPos, sTime, l)
{
}

string InstantGlowLineEffect::GetTypeName()
{
	return "GlowLineEffect";
}
