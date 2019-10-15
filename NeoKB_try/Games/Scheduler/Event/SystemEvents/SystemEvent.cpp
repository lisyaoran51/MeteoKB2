#include "SystemEvent.h"


using namespace Games::Schedulers::Events::SystemEvents;

SystemEvent::SystemEvent() : Event()
{
}

SystemEvent::SystemEvent(MTO_FLOAT s, MTO_FLOAT l) : Event(s,l)
{
}

string SystemEvent::GetTypeName()
{
	return "SystemEvent";
}
