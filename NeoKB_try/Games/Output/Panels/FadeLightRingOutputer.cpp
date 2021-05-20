#include "FadeLightRingOutputer.h"


using namespace Games::Output::Panels;


int FadeLightRingOutputer::SetupPeripheral(MainInterface * mainInterface)
{

	panel = mainInterface->GetPanel();

	return 0;
}

int FadeLightRingOutputer::pushMessage(FadeLightRingPanelMessage * outputMessage)
{
	LOG(LogLevel::Depricated) << "FadeLightRingOutputer::pushMessage : get message.";

	if (panel)
		panel->PushOutputMessage(outputMessage);
	else {
		LOG(LogLevel::Error) << "FadeLightRingOutputer::pushMessage : no panel device registered.";
		// throw error
	}

	return 0;
}
