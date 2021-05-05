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

int SustainPedalLightRingIoCommunicator::implementProcessIO(EventProcessor<Event>* sPedalLightRing)
{
	LOG(LogLevel::Depricated) << "SustainPedalLightRingIoCommunicator::implementProcessIO : start send i2c.";

	if (!outputManager)
		return -1;

	if (!dynamic_cast<SustainPedalLightRing*>(sPedalLightRing))
		return -1;

	SustainPedalLightRing* sustainPedalLightRing = dynamic_cast<SustainPedalLightRing*>(sPedalLightRing);

	//if (!sustainPedalLightRing->GetIsTransferable())
	//	return -1;
	SustainPedalLightRingPanelMessage* message = nullptr;

	if (sustainPedalLightRing->GetHasNotifyInAdvance()) {
		message = new SustainPedalLightRingPanelMessage(
			sustainPedalLightRing->GetTargetLifeTime(),
			sustainPedalLightRing->GetPedalDownLifeTime(),
			sustainPedalLightRing->GetRingLifeTime()
		);
	}
	else {
		message = new SustainPedalLightRingPanelMessage(
			0,
			sustainPedalLightRing->GetLifeTime(),
			sustainPedalLightRing->GetLifeTime()
		);
	}



	LOG(LogLevel::Debug) << "SustainPedalLightRingIoCommunicator::implementProcessIO : send i2c [" << message->ToString() << "] at [" << sustainPedalLightRing->GetTargetStartTime() << "].";


	//sustainPedalLightRing->SetIsTransfered();

	outputManager->PushMessage(message);

	return 0;
}
