#include "StopSystemEvent.h"


using namespace Base::Schedulers::Events::SystemEvents;

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
