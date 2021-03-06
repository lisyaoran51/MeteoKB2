#include "InstantPianoSoundEvent.h"


using namespace Instant::Schedulers::Events::InstrumentEvents;


InstantPianoSoundEvent::InstantPianoSoundEvent(pair<Pitch, float> input, MTO_FLOAT s, MTO_FLOAT l) : InstrumentEvent(s, l)
{
	pianoSoundEventType = InstantPianoSoundEventType::Pitch;
	sound = input;
}

InstantPianoSoundEvent::InstantPianoSoundEvent(bool pDown, MTO_FLOAT s, MTO_FLOAT l) : InstrumentEvent(s, l)
{
	pianoSoundEventType = InstantPianoSoundEventType::Pedal;
	pedalDown = pDown;
}

pair<Pitch, float> InstantPianoSoundEvent::GetSound()
{
	if (pianoSoundEventType != InstantPianoSoundEventType::Pitch)
		throw runtime_error("PianoSoundEvent::GetSound() : wrong PianoSoundEventType.");

	return sound;
}

bool InstantPianoSoundEvent::GetPedalDown()
{
	if (pianoSoundEventType != InstantPianoSoundEventType::Pedal)
		throw runtime_error("PianoSoundEvent::GetPedalDown() : wrong PianoSoundEventType.");

	return pedalDown;
}

string InstantPianoSoundEvent::GetTypeName()
{
	return "PianoSoundEvent";
}

InstantPianoSoundEventType InstantPianoSoundEvent::GetPianoSoundEventType()
{
	return pianoSoundEventType;
}
