#include "SpeedRingOutputer.h"


using namespace Games::Output::Panels;


int SpeedRingOutputer::SetupPeripheral(MainInterface * mainInterface)
{
	panel = mainInterface->GetPanel();
	return 0;
}

int SpeedRingOutputer::pushMessage(SpeedRingPanelMessage * outputMessage)
{
	if (panel)
		panel->PushOutputMessage(outputMessage);
	else {
		LOG(LogLevel::Error) << "SpeedRingOutputer::pushMessage : no panel device registered.";
		// throw error
	}
	return 0;
}
