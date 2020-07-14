#include "RepeatPracticeModifier.h"

#include "../../Timing/MeteorTimeController.h"


using namespace Meteor::Rulesets::Modifiers;
using namespace Meteor::Timing;



RepeatPracticeModifier::RepeatPracticeModifier(int rSection, int rTimes) : RegisterType("RepeatPracticeModifier")
{
	repeatSections = rSection;
	repeatTimes = rTimes;
}

int RepeatPracticeModifier::ApplyToTimeController(TimeController * timeController)
{
	MeteorTimeController* meteorTimeController = dynamic_cast<MeteorTimeController*>(timeController);

	if (!meteorTimeController)
		throw runtime_error("RepeatPracticeModifier::ApplyToTimeController() : time controller conversion error");

	meteorTimeController->SetTimeControllerMode(MeteorTimeControllerMode::RepeatPractice);

	meteorTimeController->SetRepeatSections(repeatSections);

	meteorTimeController->SetRepeatTimes(repeatTimes);

	return 0;
}