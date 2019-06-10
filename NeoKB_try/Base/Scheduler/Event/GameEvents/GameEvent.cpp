#include "GameEvent.h"


using namespace Base::Schedulers::Events::GameEvents;



GameEvent::GameEvent(MTO_FLOAT s, MTO_FLOAT l) : Event(s,l)
{

}

string GameEvent::GetTypeName()
{
	return "GameEvent";
}
