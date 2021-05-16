#include "PianoSoundEvent.h"


using namespace Meteor::Schedulers::Events::InstrumentEvents;


PianoSoundEvent::PianoSoundEvent(pair<Pitch, float> input, MTO_FLOAT s, MTO_FLOAT l): InstrumentEvent(s, l)
{
	pianoSoundEventType = PianoSoundEventType::Pitch;
	sound = input;
}

PianoSoundEvent::PianoSoundEvent(bool pDown, MTO_FLOAT s, MTO_FLOAT l) : InstrumentEvent(s, l)
{
	pianoSoundEventType = PianoSoundEventType::Pedal;
	pedalDown = pDown;
}

pair<Pitch, float> PianoSoundEvent::GetSound()
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

Event * PianoSoundEvent::Clone()
{
	PianoSoundEvent* newEvent;
	if (pianoSoundEventType == PianoSoundEventType::Pitch) {
		newEvent = new PianoSoundEvent(sound, startTime, lifeTime);
	}
	else {
		newEvent = new PianoSoundEvent(pedalDown, startTime, lifeTime);
	}
	newEvent->SetSourceEvent(sourceEvent);
	return newEvent;
}

PianoSoundEventType PianoSoundEvent::GetPianoSoundEventType()
{
	return pianoSoundEventType;
}
