#include "RepeatPracticeController.h"


#include "../../TimeEvents/RepeatPracticeEventProcessor.h"


using namespace Meteor::Schedulers::Events::TimeEvents::TimeControllerControllers;
using namespace Meteor::Schedulers::Events::TimeEvents;


int RepeatPracticeController::load()
{
	OutputManager* o = GetCache<OutputManager>("OutputManager");

	if (!o)
		throw runtime_error("int RepeatPracticeController::load() : OutputManager not found in cache.");
	
	return load(o);
}

int RepeatPracticeController::load(OutputManager * o)
{
	outputManager = o;

	meteorTimeController = dynamic_cast<MeteorTimeController*>(timeController);
	if (!meteorTimeController)
		throw runtime_error("int RepeatPracticeController::load() : MeteorTimeController not found in cache.");

	return 0;
}

RepeatPracticeController::RepeatPracticeController(): RegisterType("RepeatPracticeController")
{
	registerLoad(bind(static_cast<int(RepeatPracticeController::*)(void)>(&RepeatPracticeController::load), this));
}

int RepeatPracticeController::implementControlTimeController(EventProcessor<Event>* e)
{
	LOG(LogLevel::Debug) << "RepeatPracticeController::implementControlTimeController() : repeat practice event [" << e << "].";
	meteorTimeController->RepeatSection(dynamic_cast<RepeatPracticeEventProcessor*>(e)->GetRepeatSection());

	return 0;
}
