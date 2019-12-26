#include "SustainPedalLightRingIoCommunicator.h"

#include "../../../../Output/Panels/SustainPedalLightRingPanelMessage.h"


using namespace Meteor::Schedulers::Events::IoEvents::IoCommunicators;
using namespace Meteor::Output::Panels;



int SustainPedalLightRingIoCommunicator::load()
{

	return 0;
}


SustainPedalLightRingIoCommunicator::SustainPedalLightRingIoCommunicator():RegisterType("SustainPedalLightRingIoCommunicator")
{
	registerLoad(bind((int(SustainPedalLightRingIoCommunicator::*)())&SustainPedalLightRingIoCommunicator::load, this));
}

int SustainPedalLightRingIoCommunicator::implementProcessIO(SustainPedalLightRing * sPedalLightRing)
{
	if (!outputManager)
		return -1;

	if (!sPedalLightRing->GetIsTransferable())
		return -1;

	SustainPedalLightRingPanelMessage* message = new SustainPedalLightRingPanelMessage(
		sPedalLightRing->GetTargetLifeTime(),
		sPedalLightRing->GetPedalDownLifeTime(),
		sPedalLightRing->GetRingLifeTime()
	);

	sPedalLightRing->SetIsTransfered();

	outputManager->PushMessage(message);

	return 0;
}
