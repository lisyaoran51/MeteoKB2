#ifndef PIANO_EVENT_PROCESSOR_H
#define PIANO_EVENT_PROCESSOR_H


#include "../../../../Games/Scheduler/Event/InstrumentEvents/InstrumentEventProcessor.h"
#include "PianoEvent.h"
#include <utility>

using namespace Games::Schedulers::Events::InstrumentEvents;
using namespace std;


namespace Meteor {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{

	class PianoEventProcessor : public InstrumentEventProcessor<PianoEvent> {

	public:

		pair<InputKey, int> GetInput();


	};

}}}}


#endif