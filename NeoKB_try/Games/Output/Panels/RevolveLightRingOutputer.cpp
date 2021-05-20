#include "RevolveLightRingOutputer.h"


using namespace Games::Output::Panels;


int RevolveLightRingOutputer::SetupPeripheral(MainInterface * mainInterface)
{

	panel = mainInterface->GetPanel();

	return 0;
}

int RevolveLightRingOutputer::pushMessage(RevolveLightRingPanelMessage * outputMessage)
{
	LOG(LogLevel::Depricated) << "RevolveLightRingOutputer::pushMessage : get message.";

	if (panel)
		panel->PushOutputMessage(outputMessage);
	else {
		LOG(LogLevel::Error) << "RevolveLightRingOutputer::pushMessage : no panel device registered.";
		// throw error
	}

	return 0;
}
