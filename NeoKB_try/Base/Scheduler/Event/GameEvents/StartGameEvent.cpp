#include "StartGameEvent.h"


using namespace Base::Schedulers::Events::GameEvents;


StartGameEvent::StartGameEvent(MTO_FLOAT s, MTO_FLOAT l) : GameEvent(s,l)
{
}

GameEventType StartGameEvent::GetGameEventType()
{
	return GameEventType::Start;
}

string StartGameEvent::GetTypeName()
{
	return "StartGameEvent";
}

Event * StartGameEvent::Clone()
{
	return new StartGameEvent(startTime, lifeTime);
}
