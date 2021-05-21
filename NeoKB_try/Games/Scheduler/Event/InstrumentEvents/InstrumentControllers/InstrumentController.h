#ifndef INSTRUMENT_CONTROLLER_H
#define INSTRUMENT_CONTROLLER_H


#include "../../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../EventProcessor.h"
#include "../../../../../Instruments/Instrument.h"



using namespace Framework::Allocation::Hierachal;
using namespace Games::Schedulers::Events;
using namespace Instruments;



namespace Games {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{
namespace InstrumentControllers{

	class InstrumentControllerInterface : public Container {

	public:

		InstrumentControllerInterface();

		virtual int ControlInstrument(EventProcessor<Event>* e) = 0;

		virtual int FastForwardControlInstrument(EventProcessor<Event>* e) = 0;

		virtual int UndoControlInstrument(EventProcessor<Event>* e) = 0;

	};

	template<typename T>
	class InstrumentController : public InstrumentControllerInterface {

		int load() {

			LOG(LogLevel::Info) << "IoCommunicator::load() : Start loading config.";

			FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");

			if (!f)
				throw runtime_error("int IoCommunicator::load() : FrameworkConfigManager not found in cache.");

			Instrument* i = GetCache<Instrument>("Instrument");

			return load(f, i);
		}

		int load(FrameworkConfigManager* f, Instrument* i) {

			instrument = i;

			return 0;
		}

	public:

		InstrumentController(): RegisterType("InstrumentController") {
			registerLoad(bind((int(InstrumentController<T>::*)())&InstrumentController<T>::load, this));
		}

		virtual int ControlInstrument(EventProcessor<Event>* e) {
			return implementControlInstrument(e);
		}

		virtual int FastForwardControlInstrument(EventProcessor<Event>* e) {
			return implementFastForwardControlInstrument(e);
		}

		virtual int UndoControlInstrument(EventProcessor<Event>* e) {
			return implementUndoControlInstrument(e);
		}


	protected:

		Instrument* instrument = nullptr;

		virtual int implementControlInstrument(EventProcessor<Event>* e) = 0;

		virtual int implementFastForwardControlInstrument(EventProcessor<Event>* e) = 0;

		virtual int implementUndoControlInstrument(EventProcessor<Event>* e) = 0;

	};

}}}}}

#endif