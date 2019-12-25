#include "SustainPedalLightRing.h"


using namespace Meteor::Schedulers::Events::IoEvents;



MTO_FLOAT SustainPedalLightRing::GetTargetStartTime()
{
	return GetIoEvent()->GetTargetStartTime();
}

MTO_FLOAT SustainPedalLightRing::GetTargetLifeTime()
{
	return GetIoEvent()->GetTargetLifeTime();
}

MTO_FLOAT SustainPedalLightRing::GetRingLifeTime()
{
	return MTO_FLOAT();
}

MTO_FLOAT SustainPedalLightRing::GetNextTargetStartTime()
{
	return GetIoEvent()->GetNextTargetStartTime();
}
