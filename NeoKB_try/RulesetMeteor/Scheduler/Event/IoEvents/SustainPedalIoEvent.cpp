#include "SustainPedalIoEvent.h"



using namespace Meteor::Schedulers::Events::IoEvents;



SustainPedalIoEvent::SustainPedalIoEvent(InputKey iKey, MTO_FLOAT s, MTO_FLOAT l): PanelEvent(iKey, s, l)
{
}

int SustainPedalIoEvent::SetTargetTime(MTO_FLOAT tStartTime, MTO_FLOAT tLifeTime, MTO_FLOAT rLifeTime, MTO_FLOAT nTargetStartTime)
{
	targetStartTime = tStartTime;
	targetLifeTime = tLifeTime;
	ringLifeTime = rLifeTime;
	nextTargetStartTime = nTargetStartTime;

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

MTO_FLOAT SustainPedalIoEvent::GetRingLifeTime()
{
	return ringLifeTime;
}

MTO_FLOAT SustainPedalIoEvent::GetNextTargetStartTime()
{
	return nextTargetStartTime;
}
