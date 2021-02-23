#include "RestartSystemEvent.h"


using namespace Games::Schedulers::Events::SystemEvents;


RestartSystemEvent::RestartSystemEvent()
{
}

RestartSystemEvent::RestartSystemEvent(MTO_FLOAT s, MTO_FLOAT l): SystemEvent(s, l)
{
}

SystemEventType RestartSystemEvent::GetSystemEventType()
{
	return SystemEventType::Restart;
}

string RestartSystemEvent::GetTypeName()
{
	return "RestartSystemEvent";
}
