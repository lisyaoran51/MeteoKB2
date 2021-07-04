#ifndef INSTANT_PIANO_EVENT_PROCESSOR_H
#define INSTANT_PIANO_EVENT_PROCESSOR_H


#include "../../../../Games/Scheduler/Event/InstrumentEvents/InstrumentEventProcessor.h"
#include "InstantPianoEvent.h"
#include <utility>

using namespace Games::Schedulers::Events::InstrumentEvents;
using namespace std;


namespace Instant {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{

	class InstantPianoEventProcessor : public InstrumentEventProcessor<InstantPianoEvent> {

	public:

		pair<InputKey, int> GetInput();


	};

}}}}


#endif