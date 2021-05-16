#include "OctaveShiftEvent.h"


using namespace Meteor::Schedulers::Events::PlayfieldEvents;


OctaveShiftEvent::OctaveShiftEvent(OctaveShiftType oShiftType, MTO_FLOAT s, MTO_FLOAT l): PlayfieldEvent(s, l)
{
	octaveShiftType = oShiftType;
}

string OctaveShiftEvent::GetTypeName()
{
	return "OctaveShiftEvent";
}

Event * OctaveShiftEvent::Clone()
{
	OctaveShiftEvent* newEvent = new OctaveShiftEvent(octaveShiftType, startTime, lifeTime);
	newEvent->SetSourceEvent(sourceEvent);
	return newEvent;
}

OctaveShiftType OctaveShiftEvent::GetShiftType()
{
	return octaveShiftType;
}

float OctaveShiftEvent::GetShiftStartTime()
{
	return GetStartTime();
}

float OctaveShiftEvent::GetShiftTimeLength()
{
	return GetLifeTime();
}
