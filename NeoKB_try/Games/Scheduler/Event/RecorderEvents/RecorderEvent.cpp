#include "RecorderEvent.h"


using namespace Games::Schedulers::Events::RecorderEvents;


RecorderEvent::RecorderEvent(MTO_FLOAT s, MTO_FLOAT l) : Event(s, l)
{
}

string RecorderEvent::GetTypeName()
{
	return "RecorderEvent";
}
