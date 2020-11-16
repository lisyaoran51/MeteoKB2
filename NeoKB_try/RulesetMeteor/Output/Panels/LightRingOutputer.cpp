#include "LightRingOutputer.h"


using namespace Meteor::Output::Panels;


int LightRingOutputer::SetupPeripheral(MainInterface * mainInterface)
{

	panel = mainInterface->GetPanel();

	return 0;
}

int LightRingOutputer::pushMessage(LightRingPanelMessage * outputMessage)
{
	LOG(LogLevel::Depricated) << "LightRingOutputer::pushMessage : get message.";

	if (panel)
		panel->PushOutputMessage(outputMessage);
	else {
		LOG(LogLevel::Error) << "LightRingOutputer::pushMessage : no panel device registered.";
		// throw error
	}

	return 0;
}
