#ifndef TIME_CONRTOLLER_CONRTOLLER_H
#define TIME_CONRTOLLER_CONRTOLLER_H


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
namespace TimeConrtollerControllers {


	class TimeControllerConrtollerInterface : public Container {

	public:

		TimeControllerConrtollerInterface();

		virtual int LazyConstruct(TimeController* t) = 0;

		virtual int ControlTimeConrtoller(EventProcessor<Event>* eProcessor) = 0;

	protected:


	};

	template<typename T>
	class TimeControllerConrtoller : public TimeControllerConrtollerInterface {

		int load() {

			LOG(LogLevel::Info) << "TimeControllerConrtoller::load() : Start loading config.";

			FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");

			if (!f)
				throw runtime_error("int PlayfieldController::load() : FrameworkConfigManager not found in cache.");

			return load(f);
		}

		int load(FrameworkConfigManager* f) {

			return 0;
		}


	public:

		TimeControllerConrtoller() : RegisterType("TimeControllerConrtoller") {
			registerLoad(bind((int(TimeControllerConrtoller<T>::*)())&TimeControllerConrtoller<T>::load, this));
		}

		virtual int LazyConstruct(TimeController* t) {
			timeController = t;
			return 0;
		}

		virtual int ControlTimeConrtoller(EventProcessor<Event>* eProcessor) {

			return implementControlTimeConrtoller(eProcessor);

		}

	protected:

		virtual int implementControlTimeConrtoller(EventProcessor<Event>* eProcessor) = 0;

		TimeController* timeController = nullptr;

	};



}}}}}


#endif