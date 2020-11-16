#include "AutoPedalModifier.h"

#include "../../../Instruments/Piano.h"

using namespace Meteor::Rulesets::Modifiers;
using namespace Instruments;



AutoPedalModifier::AutoPedalModifier(): RegisterType("AutoPedalModifier")
{
	description = "AutoPedal";
	//modifierType = ModifierType::Special;
	ranked = true;
}

int AutoPedalModifier::ApplyToInstrument(Instrument* instrument)
{
	LOG(LogLevel::Debug) << "AutoPedalModifier::ApplyToInstrument() : make piano pedal controlled by game.";
	LOG(LogLevel::Error) << "AutoPedalModifier::ApplyToInstrument() : this modifier should be deleted? only manual pedal modifier should left?";

	if (dynamic_cast<Piano*>(instrument)) {

		dynamic_cast<Piano*>(instrument)->ChangeSustainType(SustainType::GameControllingSustain);

	}
	return 0;
}
