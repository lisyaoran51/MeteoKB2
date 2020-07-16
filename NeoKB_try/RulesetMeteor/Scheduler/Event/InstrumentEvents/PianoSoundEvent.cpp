#include "PianoSoundEvent.h"


using namespace Meteor::Schedulers::Events::InstrumentEvents;


PianoSoundEvent::PianoSoundEvent(pair<Pitch, int> input, MTO_FLOAT s, MTO_FLOAT l): InstrumentEvent(s, l)
{
	pianoSoundEventType = PianoSoundEventType::Pitch;
	sound = input;
}

PianoSoundEvent::PianoSoundEvent(bool pDown, MTO_FLOAT s, MTO_FLOAT l) : InstrumentEvent(s, l)
{
	pianoSoundEventType = PianoSoundEventType::Pedal;
	pedalDown = pDown;
}

pair<Pitch, int> PianoSoundEvent::GetSound()
{
	if (pianoSoundEventType != PianoSoundEventType::Pitch)
		throw runtime_error("PianoSoundEvent::GetSound() : wrong PianoSoundEventType.");

	return sound;
}

bool PianoSoundEvent::GetPedalDown()
{
	if (pianoSoundEventType != PianoSoundEventType::Pedal)
		throw runtime_error("PianoSoundEvent::GetPedalDown() : wrong PianoSoundEventType.");

	return pedalDown;
}

string PianoSoundEvent::GetTypeName()
{
	return "PianoSoundEvent";
}

PianoSoundEventType PianoSoundEvent::GetPianoSoundEventType()
{
	return pianoSoundEventType;
}
