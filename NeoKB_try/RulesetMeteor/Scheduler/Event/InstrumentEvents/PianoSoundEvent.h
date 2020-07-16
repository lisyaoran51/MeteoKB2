#ifndef PIANO_SOUND_EVENT_H
#define PIANO_SOUND_EVENT_H


#include "../../../../Games/Scheduler/Event/InstrumentEvents/InstrumentEvent.h"
#include "../../../../Instruments/Pitch.h"


using namespace Games::Schedulers::Events::InstrumentEvents;
using namespace Instruments;


namespace Meteor {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{

	enum class PianoSoundEventType {
		None,
		Pitch,
		Pedal
	};

	class PianoSoundEvent : public InstrumentEvent {

	public:

		PianoSoundEvent(pair<Pitch, float> input, MTO_FLOAT s, MTO_FLOAT l);

		PianoSoundEvent(bool pDown, MTO_FLOAT s, MTO_FLOAT l);

		// 一定要每次都override!!
		virtual string GetTypeName();

		PianoSoundEventType GetPianoSoundEventType();

		pair<Pitch, float> GetSound();

		bool GetPedalDown();

	protected:

		PianoSoundEventType pianoSoundEventType = PianoSoundEventType::None;

		pair<Pitch, float> sound = pair<Pitch, float>();

		bool pedalDown = false;

	};

}}}}



#endif