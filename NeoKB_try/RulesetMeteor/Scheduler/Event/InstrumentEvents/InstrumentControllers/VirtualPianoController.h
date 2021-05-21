#ifndef VIRTUAL_PIANO_CONTROLLER_H
#define VIRTUAL_PIANO_CONTROLLER_H

#include "../PianoSoundEvent.h"
#include "../../../../../Games/Scheduler/Event/InstrumentEvents/InstrumentControllers/InstrumentController.h"
#include "../../../../../Instruments/VirtualMeteoPiano.h"


using namespace Games::Schedulers::Events::InstrumentEvents::InstrumentControllers;
using namespace Instruments;


namespace Meteor {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{
namespace InstrumentControllers{

	class VirtualPianoController : public InstrumentController<PianoSoundEvent> {

		int load();

	public:

		VirtualPianoController();

	protected:

		VirtualMeteoPiano* piano = nullptr;

		virtual int implementControlInstrument(EventProcessor<Event>* e);

		virtual int implementFastForwardControlInstrument(EventProcessor<Event>* e);

		virtual int implementUndoControlInstrument(EventProcessor<Event>* e);

	};

}}}}}







#endif