#include "TimeEvent.h"


using namespace Games::Schedulers::Events::TimeEvents;


TimeEvent::TimeEvent(MTO_FLOAT s, MTO_FLOAT l):Event(s, l)
{
}

string TimeEvent::GetTypeName()
{
	return "TimeEvent";
}
