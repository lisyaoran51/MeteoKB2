#include "SustainPedalLightRing.h"


using namespace Meteor::Schedulers::Events::IoEvents;



SustainPedalLightRing::SustainPedalLightRing()
{
	ioTransferType = IoTransferType::Once;
}

MTO_FLOAT SustainPedalLightRing::GetStartTime()
{
	return GetIoEvent()->GetTargetStartTime();
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
