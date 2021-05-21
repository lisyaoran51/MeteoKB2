#ifndef INSTANT_VIRTUAL_PIANO_CONTROLLER_H
#define INSTANT_VIRTUAL_PIANO_CONTROLLER_H

#include "../InstantPianoSoundEvent.h"
#include "../../../../../Games/Scheduler/Event/InstrumentEvents/InstrumentControllers/InstrumentController.h"
#include "../../../../../Instruments/VirtualMeteoPiano.h"


using namespace Games::Schedulers::Events::InstrumentEvents::InstrumentControllers;
using namespace Instruments;


namespace Instant {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{
namespace InstrumentControllers{

	class InstantVirtualPianoController : public InstrumentController<InstantPianoSoundEvent> {

		int load();

	public:

		InstantVirtualPianoController();

	protected:

		VirtualMeteoPiano* piano = nullptr;

		virtual int implementControlInstrument(EventProcessor<Event>* e);

		virtual int implementFastForwardControlInstrument(EventProcessor<Event>* e);

		virtual int implementUndoControlInstrument(EventProcessor<Event>* e);

	};

}}}}}







#endif