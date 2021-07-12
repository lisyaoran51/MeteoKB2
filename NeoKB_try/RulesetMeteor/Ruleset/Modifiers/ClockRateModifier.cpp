#include "ClockRateModifier.h"



using namespace Meteor::Rulesets::Modifiers;


ClockRateModifier::ClockRateModifier() : RegisterType("ClockRateModifier")
{
}

ClockRateModifier::ClockRateModifier(float cRate) : RegisterType("ClockRateModifier")
{
	clockRate = cRate;
}

int ClockRateModifier::ApplyToTimeController(TimeController * timeController)
{
	timeController->SetRate(clockRate);
	return 0;
}

int ClockRateModifier::SetValue(float value1, float value2)
{
	if(value1 >= 0.4 && value1 <= 1.6)
		clockRate = value1;
	return 0;
}
