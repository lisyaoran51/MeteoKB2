#include "SustainPedalLightRingIoCommunicator.h"

#include "../../../../Output/Panels/SustainPedalLightRingPanelMessage.h"
#include "../SustainPedalLightRing.h"


using namespace Meteor::Schedulers::Events::IoEvents::IoCommunicators;
using namespace Meteor::Output::Panels;
using namespace Meteor::Schedulers::Events::IoEvents;



int SustainPedalLightRingIoCommunicator::load()
{

	return 0;
}


SustainPedalLightRingIoCommunicator::SustainPedalLightRingIoCommunicator():RegisterType("SustainPedalLightRingIoCommunicator")
{
	registerLoad(bind((int(SustainPedalLightRingIoCommunicator::*)())&SustainPedalLightRingIoCommunicator::load, this));
}

int SustainPedalLightRingIoCommunicator::implementProcessIO(IoEventProcessor<SustainPedalIoEvent>* sPedalLightRing)
{
	if (!outputManager)
		return -1;

	if (!sPedalLightRing->GetIsTransferable())
		return -1;

	if (!dynamic_cast<SustainPedalLightRing*>(sPedalLightRing))
		return -1;

	SustainPedalLightRing* sustainPedalLightRing = dynamic_cast<SustainPedalLightRing*>(sPedalLightRing);

	SustainPedalLightRingPanelMessage* message = new SustainPedalLightRingPanelMessage(
		sustainPedalLightRing->GetTargetLifeTime(),
		sustainPedalLightRing->GetPedalDownLifeTime(),
		sustainPedalLightRing->GetRingLifeTime()
	);

	sustainPedalLightRing->SetIsTransfered();

	outputManager->PushMessage(message);

	return 0;
}
