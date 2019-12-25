#include "SustainPedalLightRingOutputer.h"


using namespace Meteor::Output::Panels;


int SustainPedalLightRingOutputer::SetupPeripheral(MainInterface * mainInterface)
{

	panel = mainInterface->GetPanel();

	return 0;
}

int SustainPedalLightRingOutputer::pushMessage(SustainPedalLightRingPanelMessage * outputMessage)
{
	if(panel)
		panel->PushOutputMessage(outputMessage);
	else {
		// throw error
	}

	return 0;
}
