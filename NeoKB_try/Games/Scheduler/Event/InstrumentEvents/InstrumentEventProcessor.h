#ifndef INSTRUMENT_EVENT_PROCESSOR_H
#define INSTRUMENT_EVENT_PROCESSOR_H


#include "../EventProcessor.h"
#include "InstrumentControllers/InstrumentController.h"

using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::InstrumentEvents::InstrumentControllers;

namespace Games {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{

	class InstrumentEventProcessorInterface : public EventProcessor<Event> {

	public:

		virtual int RegisterInstrumentController(InstrumentControllerInterface* iController) = 0;

		virtual int ControlInstrument() = 0;

		virtual int SetIsTransfered() = 0;

		virtual bool GetIsTransferable() = 0;

	};

	template<typename T>
	class InstrumentEventProcessor : public InstrumentEventProcessorInterface {



	public:

		InstrumentEventProcessor(){
		}

		virtual int RegisterInstrumentController(InstrumentControllerInterface* iController) {
			if (MtoObject::CanCast<InstrumentController<T>>(iController)) {
				instrumentController = MtoObject::Cast<InstrumentController<T>>(iController);
				return 0;
			}
			// TODO: ¼Q¿ù»~
			return -1;
		}

		virtual int ControlInstrument() {
			if (!instrumentController)
				LOG(LogLevel::Error) << "InstrumentEventProcessor::ControlInstrument : no instruemnt controller.";

			if (instrumentController && GetIsTransferable()) {

				SetIsTransfered();
				instrumentController->ControlInstrument(this);

			}
			return 0;
		}

		virtual int SetIsTransfered() {
			isTransferable = false;
		}

		virtual bool GetIsTransferable() {
			return isTransferable;
		}

		T* GetInstrumentEvent() { return dynamic_cast<T*>(event); }

	protected:

		bool isTransferable = true;

		InstrumentController<T>* instrumentController = nullptr;

	};


}}}}


#endif