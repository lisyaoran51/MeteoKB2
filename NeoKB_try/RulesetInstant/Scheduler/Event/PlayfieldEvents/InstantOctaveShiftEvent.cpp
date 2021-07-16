#include "InstantOctaveShiftEvent.h"


using namespace Instant::Schedulers::Events::PlayfieldEvents;


InstantOctaveShiftEvent::InstantOctaveShiftEvent(InstantOctaveShiftType oShiftType, MTO_FLOAT s, MTO_FLOAT l): PlayfieldEvent(s, l)
{
	octaveShiftType = oShiftType;
	lifeType = EventLifeType::Immediate;
}

string InstantOctaveShiftEvent::GetTypeName()
{
	return "InstantOctaveShiftEvent";
}

Event * InstantOctaveShiftEvent::Clone()
{
	InstantOctaveShiftEvent* newEvent = new InstantOctaveShiftEvent(octaveShiftType, startTime, lifeTime);
	newEvent->SetSourceEvent(sourceEvent);
	return newEvent;
}

InstantOctaveShiftType InstantOctaveShiftEvent::GetShiftType()
{
	return octaveShiftType;
}

float InstantOctaveShiftEvent::GetShiftStartTime()
{
	return GetStartTime();
}
