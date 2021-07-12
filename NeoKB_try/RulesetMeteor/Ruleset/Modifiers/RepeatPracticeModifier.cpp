#include "RepeatPracticeModifier.h"

#include "../../Timing/MeteorTimeController.h"


using namespace Meteor::Rulesets::Modifiers;
using namespace Meteor::Timing;



RepeatPracticeModifier::RepeatPracticeModifier() : RegisterType("RepeatPracticeModifier")
{
}

RepeatPracticeModifier::RepeatPracticeModifier(int rSection, int rTimes) : RegisterType("RepeatPracticeModifier")
{
	maxSectionAmountForOneRepeat = rSection;
	maxRepeatCounts = rTimes;
}

int RepeatPracticeModifier::ApplyToTimeController(TimeController * timeController)
{
	MeteorTimeController* meteorTimeController = dynamic_cast<MeteorTimeController*>(timeController);

	if (!meteorTimeController)
		throw runtime_error("RepeatPracticeModifier::ApplyToTimeController() : time controller conversion error");

	meteorTimeController->SetTimeControllerMode(MeteorTimeControllerMode::RepeatPractice);

	meteorTimeController->SetRepeatSections(maxSectionAmountForOneRepeat);

	meteorTimeController->SetRepeatTimes(maxRepeatCounts);

	return 0;
}

int RepeatPracticeModifier::SetValue(float value1, float value2)
{
	maxSectionAmountForOneRepeat = (int)value1;
	maxRepeatCounts = (int)value2;
	return 0;
}
