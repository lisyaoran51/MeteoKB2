#include "EndSystemEvent.h"


using namespace Games::Schedulers::Events::SystemEvents;

EndSystemEvent::EndSystemEvent() : SystemEvent()
{
}


EndSystemEvent::EndSystemEvent(MTO_FLOAT s, MTO_FLOAT l) : SystemEvent(s, l)
{
}

SystemEventType EndSystemEvent::GetSystemEventType()
{
	return SystemEventType::End;
}

string EndSystemEvent::GetTypeName()
{
	return "EndSystemEvent";
}
