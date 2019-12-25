#include "SustainPedalIoEvent.h"



using namespace Meteor::Schedulers::Events::IoEvents;



SustainPedalIoEvent::SustainPedalIoEvent(InputKey iKey, MTO_FLOAT s, MTO_FLOAT l): PanelEvent(iKey, s, l)
{
}

int SustainPedalIoEvent::SetTargetTime(MTO_FLOAT tStartTime, MTO_FLOAT tLifeTime)
{
	targetStartTime = tStartTime;
	targetLifeTime = tLifeTime;

	return 0;
}

MTO_FLOAT SustainPedalIoEvent::GetTargetStartTime()
{
	return targetStartTime;
}

MTO_FLOAT SustainPedalIoEvent::GetTargetLifeTime()
{
	return targetLifeTime;
}
