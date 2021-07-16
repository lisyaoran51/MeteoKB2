#ifndef INSTANT_PIANO_SOUND_EVENT_H
#define INSTANT_PIANO_SOUND_EVENT_H


#include "../../../../Games/Scheduler/Event/InstrumentEvents/InstrumentEvent.h"
#include "../../../../Instruments/Pitch.h"
#include "../../../../Instruments/Piano.h"


using namespace Games::Schedulers::Events::InstrumentEvents;
using namespace Instruments;


namespace Instant {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{

	enum class InstantPianoSoundEventType {
		None,
		Pitch,
		Pedal,
	};

	class InstantPianoSoundEvent : public InstrumentEvent {

	public:

		InstantPianoSoundEvent(pair<Pitch, float> input, MTO_FLOAT s, MTO_FLOAT l);

		InstantPianoSoundEvent(bool pDown, MTO_FLOAT s, MTO_FLOAT l);

		// 一定要每次都override!!
		virtual string GetTypeName();

		virtual Event* Clone();

		InstantPianoSoundEventType GetPianoSoundEventType();

		pair<Pitch, float> GetSound();

		bool GetPedalDown();

	protected:

		InstantPianoSoundEventType pianoSoundEventType = InstantPianoSoundEventType::None;

		pair<Pitch, float> sound = pair<Pitch, float>();

		bool pedalDown = false;

	};

}}}}



#endif