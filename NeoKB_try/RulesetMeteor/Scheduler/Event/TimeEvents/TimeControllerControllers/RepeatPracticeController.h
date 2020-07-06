#ifndef REPEAT_PRACTICE_CONTROLLER_H
#define REPEAT_PRACTICE_CONTROLLER_H


#include "../RepeatPracticeEvent.h"
#include "../../../../../Games/Scheduler/Event/TimeEvents/TimeControllerConrtollers/TimeControllerController.h"
#include "../../../../Timing/MeteorTimeController.h"


using namespace Games::Schedulers::Events::TimeEvents::TimeControllerControllers;
using namespace Meteor::Timing;


namespace Meteor {
namespace Schedulers{
namespace Events{
namespace TimeEvents {
namespace TimeControllerControllers {

	class RepeatPracticeController : public TimeControllerController<RepeatPracticeEvent> {

		int load();

	public:

		RepeatPracticeController();

	protected:

		MeteorTimeController* meteorTimeController = nullptr;

		virtual int implementControlTimeController(EventProcessor<Event>* e);

	};

}}}}}

#endif