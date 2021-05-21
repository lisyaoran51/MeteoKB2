#include "ChangeSpeedOutputer.h"


using namespace Games::Output::Panels;


int ChangeSpeedOutputer::SetupPeripheral(MainInterface * mainInterface)
{

	panel = mainInterface->GetPanel();

	return 0;
}

int ChangeSpeedOutputer::pushMessage(ChangeSpeedPanelMessage * outputMessage)
{
	LOG(LogLevel::Debug) << "FadeLightRingOutputer::pushMessage : get message." << outputMessage->ToString();

	if (panel)
		panel->PushOutputMessage(outputMessage);
	else {
		LOG(LogLevel::Error) << "FadeLightRingOutputer::pushMessage : no panel device registered.";
		// throw error
	}

	return 0;
}
