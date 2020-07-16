#ifndef PIANO_SOUND_EVENT_PROCESSOR_H
#define PIANO_SOUND_EVENT_PROCESSOR_H


#include "../../../../Games/Scheduler/Event/InstrumentEvents/InstrumentEventProcessor.h"
#include "PianoSoundEvent.h"
#include <utility>

using namespace Games::Schedulers::Events::InstrumentEvents;
using namespace std;


namespace Meteor {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{

	class PianoSoundEventProcessor : public InstrumentEventProcessor<PianoSoundEvent> {

	public:

		PianoSoundEventType GetPianoSoundEventType();

		pair<Pitch, int> GetSound();

		bool GetPedalDown();


	};

}}}}







#endif