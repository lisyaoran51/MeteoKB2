#include "ClockRateModifier.h"



using namespace Meteor::Rulesets::Modifiers;


ClockRateModifier::ClockRateModifier(float cRate) : RegisterType("ClockRateModifier")
{
	clockRate = cRate;
}

int ClockRateModifier::ApplyToTimeController(TimeController * timeController)
{
	timeController->SetRate(clockRate);
	return 0;
}
