#include "PlayfieldEvent.h"


using namespace Games::Schedulers::Events::PlayfieldEvents;


PlayfieldEvent::PlayfieldEvent(MTO_FLOAT s, MTO_FLOAT l): Event(s, l)
{
}

string PlayfieldEvent::GetTypeName()
{
	return "PlayfieldEvent";
}
