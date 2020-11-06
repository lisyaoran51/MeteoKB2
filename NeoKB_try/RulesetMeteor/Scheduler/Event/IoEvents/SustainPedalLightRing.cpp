#include "SustainPedalLightRing.h"


using namespace Meteor::Schedulers::Events::IoEvents;



SustainPedalLightRing::SustainPedalLightRing()
{
	ioTransferType = IoTransferType::Once;
}

bool SustainPedalLightRing::GetHasNotifyInAdvance()
{
	return GetIoEvent()->GetHasNotifyInAdvance();
}

MTO_FLOAT SustainPedalLightRing::GetStartTime()
{
	if (GetHasNotifyInAdvance())
		return GetIoEvent()->GetTargetStartTime();
	else
		return GetIoEvent()->GetStartTime();
}

MTO_FLOAT SustainPedalLightRing::GetTargetStartTime()
{
	return GetIoEvent()->GetTargetStartTime();
}

MTO_FLOAT SustainPedalLightRing::GetTargetLifeTime()
{
	return GetIoEvent()->GetTargetLifeTime();
}

MTO_FLOAT SustainPedalLightRing::GetPedalDownLifeTime()
{
	return GetIoEvent()->GetPedalDownLifeTime();
}

MTO_FLOAT SustainPedalLightRing::GetRingLifeTime()
{
	return GetIoEvent()->GetRingLifeTime();
}
