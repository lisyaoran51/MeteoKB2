#ifndef INSTANT_PIANO_SOUND_EVENT_PROCESSOR_H
#define INSTANT_PIANO_SOUND_EVENT_PROCESSOR_H


#include "../../../../Games/Scheduler/Event/InstrumentEvents/InstrumentEventProcessor.h"
#include "InstantPianoSoundEvent.h"
#include <utility>

using namespace Games::Schedulers::Events::InstrumentEvents;
using namespace std;


namespace Instant {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{

	class InstantPianoSoundEventProcessor : public InstrumentEventProcessor<InstantPianoSoundEvent> {

	public:

		InstantPianoSoundEventType GetPianoSoundEventType();

		pair<Pitch, int> GetSound();

		bool GetPedalDown();


	};

}}}}







#endif