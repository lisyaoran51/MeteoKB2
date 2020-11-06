#include "SustainPedalIoEvent.h"



using namespace Meteor::Schedulers::Events::IoEvents;



SustainPedalIoEvent::SustainPedalIoEvent(InputKey iKey, MTO_FLOAT s, MTO_FLOAT l): PanelEvent(iKey, s, l)
{
}

int SustainPedalIoEvent::SetTargetTime(MTO_FLOAT tStartTime, MTO_FLOAT tLifeTime, MTO_FLOAT pDownLifeTime, MTO_FLOAT rLifeTime)
{
	targetStartTime = tStartTime;
	targetLifeTime = tLifeTime;
	pedalDownLifeTime = pDownLifeTime;
	ringLifeTime = rLifeTime;

	return 0;
}

bool SustainPedalIoEvent::GetHasNotifyInAdvance()
{
	return hasNotifyInAdvance;
}

MTO_FLOAT SustainPedalIoEvent::GetTargetStartTime()
{
	return targetStartTime;
}

MTO_FLOAT SustainPedalIoEvent::GetTargetLifeTime()
{
	return targetLifeTime;
}

MTO_FLOAT SustainPedalIoEvent::GetPedalDownLifeTime()
{
	return pedalDownLifeTime;
}

MTO_FLOAT SustainPedalIoEvent::GetRingLifeTime()
{
	return ringLifeTime;
}

string SustainPedalIoEvent::GetTypeName()
{
	return string("SustainPedalIoEvent");
}
