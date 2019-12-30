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
		LOG(LogLevel::Error) << "SustainPedalLightRingOutputer::pushMessage : no panel device registered.";
		// throw error
	}

	return 0;
}
