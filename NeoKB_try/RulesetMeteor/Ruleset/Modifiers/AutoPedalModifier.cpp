#include "AutoPedalModifier.h"

#include "../../../Instruments/Piano.h"

using namespace Meteor::Rulesets::Modifiers;
using namespace Instruments;



AutoPedalModifier::AutoPedalModifier()
{
	name = "AutoPedal";
	description = "AutoPedal";
	modifierType = ModifierType::Special;
	ranked = true;
}

int AutoPedalModifier::ApplyToInstrument(Instrument* instrument)
{
	if (dynamic_cast<Piano*>(instrument)) {

		dynamic_cast<Piano*>(instrument)->ChangeSustainType(SustainType::GameControllingSustain);

	}
	return 0;
}
