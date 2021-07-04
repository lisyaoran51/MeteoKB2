#ifndef INSTANT_PIANO_CONTROLLER_H
#define INSTANT_PIANO_CONTROLLER_H


#include "../InstantPianoEvent.h"
#include "../../../../../Games/Scheduler/Event/InstrumentEvents/InstrumentControllers/InstrumentController.h"
#include "../../../../../Instruments/Piano.h"


using namespace Games::Schedulers::Events::InstrumentEvents::InstrumentControllers;
using namespace Instruments;


namespace Instant {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{
namespace InstrumentControllers{

	class InstantPianoController : public InstrumentController<InstantPianoEvent> {

		int load();

		int load(OutputManager* o);

	public:

		InstantPianoController();

	protected:

		Piano* piano = nullptr;

		OutputManager* outputManager = nullptr;

		virtual int implementControlInstrument(EventProcessor<Event>* e);

		virtual int implementFastForwardControlInstrument(EventProcessor<Event>* e);

		virtual int implementUndoControlInstrument(EventProcessor<Event>* e);

	};

}}}}}

#endif