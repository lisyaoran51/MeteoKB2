#include "SustainPedalLightRingOutputer.h"


using namespace Meteor::Output::Panels;


int SustainPedalLightRingOutputer::SetupPeripheral(MainInterface * mainInterface)
{

	panel = mainInterface->GetPanel();

	return 0;
}

int SustainPedalLightRingOutputer::pushMessage(SustainPedalLightRingPanelMessage * outputMessage)
{
	LOG(LogLevel::Debug) << "SustainPedalLightRingOutputer::pushMessage : get message.";

	if(panel)
		panel->PushOutputMessage(outputMessage);
	else {
		// throw error
	}

	return 0;
}
