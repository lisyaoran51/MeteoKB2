#ifndef TIME_CONTROLLER_CONTROLLER_H
#define TIME_CONTROLLER_CONTROLLER_H


#include "../../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../../../Framework/Timing/TimeController.h"
#include "../../../../Scheduler/Event/EventProcessor.h"


using namespace Framework::Allocation::Hierachal;
using namespace Framework::Timing;
using namespace Games::Schedulers::Events;


namespace Games {
namespace Schedulers {
namespace Events {
namespace TimeEvents {
namespace TimeControllerControllers {


	class TimeControllerControllerInterface : public Container {

	public:

		TimeControllerControllerInterface();

		virtual int ControlTimeController(EventProcessor<Event>* eProcessor) = 0;

	protected:


	};

	template<typename T>
	class TimeControllerController : public TimeControllerControllerInterface {

		int load() {

			LOG(LogLevel::Info) << "TimeControllerController::load() : Start loading config.";

			FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");

			if (!f)
				throw runtime_error("int PlayfieldController::load() : FrameworkConfigManager not found in cache.");

			TimeController* t = GetCache<TimeController>("TimeController");

			if (!t)
				throw runtime_error("int PlayfieldController::load() : TimeController not found in cache.");

			return load(f, t);
		}

		int load(FrameworkConfigManager* f, TimeController* t) {

			timeController = t;

			return 0;
		}


	public:

		TimeControllerController() : RegisterType("TimeControllerController") {
			registerLoad(bind((int(TimeControllerController<T>::*)())&TimeControllerController<T>::load, this));
		}

		virtual int ControlTimeController(EventProcessor<Event>* eProcessor) {

			return implementControlTimeController(eProcessor);

		}

	protected:

		virtual int implementControlTimeController(EventProcessor<Event>* eProcessor) = 0;

		TimeController* timeController = nullptr;

	};



}}}}}


#endif