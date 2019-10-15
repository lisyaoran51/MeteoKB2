#include "StopSystemEvent.h"


using namespace Games::Schedulers::Events::SystemEvents;

StopSystemEvent::StopSystemEvent() : SystemEvent()
{
}


StopSystemEvent::StopSystemEvent(MTO_FLOAT s, MTO_FLOAT l) : SystemEvent(s, l)
{
}

SystemEventType StopSystemEvent::GetSystemEventType()
{
	return SystemEventType::Stop;
}

string StopSystemEvent::GetTypeName()
{
	return "StopSystemEvent";
}
