#include "InstantPianoSoundEvent.h"


using namespace Instant::Schedulers::Events::InstrumentEvents;


InstantPianoSoundEvent::InstantPianoSoundEvent(pair<Pitch, float> input, MTO_FLOAT s, MTO_FLOAT l) : InstrumentEvent(s, l)
{
	pianoSoundEventType = InstantPianoSoundEventType::Pitch;
	sound = input;
	lifeType = EventLifeType::Immediate;
}

InstantPianoSoundEvent::InstantPianoSoundEvent(bool pDown, MTO_FLOAT s, MTO_FLOAT l) : InstrumentEvent(s, l)
{
	pianoSoundEventType = InstantPianoSoundEventType::Pedal;
	pedalDown = pDown;
}

pair<Pitch, float> InstantPianoSoundEvent::GetSound()
{
	if (pianoSoundEventType != InstantPianoSoundEventType::Pitch)
		throw runtime_error("InstantPianoSoundEvent::GetSound() : wrong PianoSoundEventType.");

	return sound;
}

bool InstantPianoSoundEvent::GetPedalDown()
{
	if (pianoSoundEventType != InstantPianoSoundEventType::Pedal)
		throw runtime_error("InstantPianoSoundEvent::GetPedalDown() : wrong PianoSoundEventType.");

	return pedalDown;
}

string InstantPianoSoundEvent::GetTypeName()
{
	return "InstantPianoSoundEvent";
}

Event * InstantPianoSoundEvent::Clone()
{
	InstantPianoSoundEvent* newEvent;
	if (pianoSoundEventType == InstantPianoSoundEventType::Pitch) {
		newEvent = new InstantPianoSoundEvent(sound, startTime, lifeTime);
	}
	else {
		newEvent = new InstantPianoSoundEvent(pedalDown, startTime, lifeTime);
	}
	
	return newEvent;
}

InstantPianoSoundEventType InstantPianoSoundEvent::GetPianoSoundEventType()
{
	return pianoSoundEventType;
}
