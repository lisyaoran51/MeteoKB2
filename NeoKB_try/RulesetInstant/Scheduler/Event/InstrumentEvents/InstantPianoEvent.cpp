#include "InstantPianoEvent.h"


using namespace Instant::Schedulers::Events::InstrumentEvents;


InstantPianoEvent::InstantPianoEvent(pair<InputKey, int> c, MTO_FLOAT s, MTO_FLOAT l) : InstrumentEvent(s, l)
{
	command = c;
	lifeType = EventLifeType::Immediate;
}

pair<InputKey, int> InstantPianoEvent::GetInput()
{
	return command;
}

string InstantPianoEvent::GetTypeName() {
	return "PianoEvent";
}

Event * InstantPianoEvent::Clone()
{
	InstantPianoEvent* newEvent = new InstantPianoEvent(command, startTime, lifeTime);
	newEvent->SetSourceEvent(sourceEvent);
	return newEvent;
}
