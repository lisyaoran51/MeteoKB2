#ifndef PIANO_CONTROLLER_H
#define PIANO_CONTROLLER_H


#include "../PianoEvent.h"
#include "../../../../../Games/Scheduler/Event/InstrumentEvents/InstrumentControllers/InstrumentController.h"
#include "../../../../../Instruments/Piano.h"


using namespace Games::Schedulers::Events::InstrumentEvents::InstrumentControllers;
using namespace Instruments;


namespace Meteor {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{
namespace InstrumentControllers{

	class PianoController : public InstrumentController<PianoEvent> {

		int load();

	public:

		PianoController();

	protected:

		Piano* piano = nullptr;

		virtual int implementControlInstrument(EventProcessor<Event>* e);

	};

}}}}}

#endif