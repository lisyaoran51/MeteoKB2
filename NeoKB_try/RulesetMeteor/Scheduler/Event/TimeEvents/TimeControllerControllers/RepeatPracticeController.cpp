#include "RepeatPracticeController.h"


#include "../../TimeEvents/RepeatPracticeEventProcessor.h"


using namespace Meteor::Schedulers::Events::TimeEvents::TimeControllerControllers;
using namespace Meteor::Schedulers::Events::TimeEvents;


int RepeatPracticeController::load()
{
	meteorTimeController = dynamic_cast<MeteorTimeController*>(timeController);
	if(!meteorTimeController)
		throw runtime_error("int RepeatPracticeController::load() : MeteorTimeController not found in cache.");

	return 0;
}

RepeatPracticeController::RepeatPracticeController(): RegisterType("RepeatPracticeController")
{
}

int RepeatPracticeController::implementControlTimeController(EventProcessor<Event>* e)
{
	meteorTimeController->RepeatSection(dynamic_cast<RepeatPracticeEventProcessor*>(e)->GetRepeatSection());

	return 0;
}
