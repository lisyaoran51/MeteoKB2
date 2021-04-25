#ifndef TIME_EVENT_PROCESSOR_H
#define TIME_EVENT_PROCESSOR_H



#include "../../../Scheduler/Event/EventProcessor.h"
#include "TimeControllerControllers/TimeControllerController.h"


using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::TimeEvents::TimeControllerControllers;



namespace Games {
namespace Schedulers {
namespace Events {
namespace TimeEvents {


	class TimeEventProcessorInterface : virtual public EventProcessor<Event> {

	public:

		virtual int RegisterTimeControllerController(TimeControllerControllerInterface* tControllerController) = 0;

		virtual int Process();

		virtual int ControllTimeController() = 0;

		virtual int SetIsProcessed(bool iProcessed = true) = 0;

		virtual bool GetIsProcessable() = 0;

	};

	template<typename T>
	class TimeEventProcessor : public TimeEventProcessorInterface {

	public:

		TimeEventProcessor() {
			eventProcessorType = EventProcessorType::Time;
		}

		virtual int RegisterTimeControllerController(TimeControllerControllerInterface* tControllerController) {
			if (MtoObject::CanCast<TimeControllerController<T>>(tControllerController)) {
				timeControllerController = MtoObject::Cast<TimeControllerController<T>>(tControllerController);
				return 0;
			}
			// TODO: ¼Q¿ù»~
			return -1;
		}

		virtual int ControllTimeController() {
			if(!timeControllerController)
				LOG(LogLevel::Error) << "TimeEventProcessor::ControllTimeController : time Controller Controller.";
				
			if (timeControllerController && GetIsProcessable()) {

				SetIsProcessed();
				timeControllerController->ControlTimeController(this);

			}
			return 0;
		}

		T* GetTimeEvent() { return dynamic_cast<T*>(event); }

		virtual int SetIsProcessed(bool iProcessed = true) {
			isProcessed = iProcessed;
			return 0;
		}

		virtual bool GetIsProcessable() {
			return !isProcessed;
		}

	protected:

	

		TimeControllerController<T>* timeControllerController = nullptr;

	};


}}}}

#endif
