#ifndef RECORDER_EVENT_PROCESSOR_H
#define RECORDER_EVENT_PROCESSOR_H


#include "../EventProcessor.h"
#include "RecorderControllers/RecorderController.h"

using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::RecorderEvents::RecorderControllers;

namespace Games {
namespace Schedulers{
namespace Events{
namespace RecorderEvents {

	class RecorderEventProcessorInterface : public EventProcessor<Event> {

	public:

		virtual int RegisterRecorderController(RecorderControllerInterface* iController) = 0;

		virtual int Process();

		virtual int AddReplay() = 0;

		virtual int SetIsTransfered() = 0;

		virtual bool GetIsTransferable() = 0;

	};

	template<typename T>
	class RecorderEventProcessor : public RecorderEventProcessorInterface {



	public:

		RecorderEventProcessor(){

			eventProcessorType = EventProcessorType::Recorder;
		}

		virtual int RegisterRecorderController(RecorderControllerInterface* rController) {
			if (MtoObject::CanCast<RecorderController<T>>(rController)) {
				recorderController = MtoObject::Cast<RecorderController<T>>(rController);
				return 0;
			}
			// TODO: ¼Q¿ù»~
			return -1;
		}

		virtual int AddReplay() {
			if (!recorderController)
				LOG(LogLevel::Error) << "RecorderEventProcessor::AddReplay : no recorder controller.";

			if (recorderController && GetIsTransferable()) {

				SetIsTransfered();
				recorderController->AddReplay(this);

			}
			return 0;
		}

		virtual int SetIsTransfered() {
			isTransferable = false;
		}

		virtual bool GetIsTransferable() {
			return isTransferable;
		}

		T* GetRecorderEvent() { return dynamic_cast<T*>(event); }

	protected:

		bool isTransferable = true;

		RecorderController<T>* recorderController = nullptr;

	};


}}}}


#endif