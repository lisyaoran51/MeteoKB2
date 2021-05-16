#include "PianoEvent.h"


using namespace Meteor::Schedulers::Events::InstrumentEvents;


PianoEvent::PianoEvent(pair<InputKey, int> c, MTO_FLOAT s, MTO_FLOAT l): InstrumentEvent(s, l)
{
	command = c;
}

pair<InputKey, int> PianoEvent::GetInput()
{
	return command;
}

string PianoEvent::GetTypeName() {
	return "PianoEvent";
}

Event * PianoEvent::Clone()
{
	PianoEvent* newEvent = new PianoEvent(command, startTime, lifeTime);
	newEvent->SetSourceEvent(sourceEvent);
	return newEvent;
}
