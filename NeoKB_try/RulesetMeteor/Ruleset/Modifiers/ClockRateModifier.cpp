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

int ClockRateModifier::SetValue(int value1, int value2)
{
	clockRate = (100.f + float(value1)) / 100.f;
	return 0;
}
