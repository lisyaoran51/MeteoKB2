#include "ChangeSpeedOutputer.h"


using namespace Games::Output::Panels;


int ChangeSpeedOutputer::SetupPeripheral(MainInterface * mainInterface)
{

	panel = mainInterface->GetPanel();

	return 0;
}

int ChangeSpeedOutputer::pushMessage(ChangeSpeedPanelMessage * outputMessage)
{
	LOG(LogLevel::Debug) << "ChangeSpeedOutputer::pushMessage : get message." << outputMessage->ToString();

	if (panel)
		panel->PushOutputMessage(outputMessage);
	else {
		LOG(LogLevel::Error) << "ChangeSpeedOutputer::pushMessage : no panel device registered.";
		// throw error
	}

	return 0;
}
