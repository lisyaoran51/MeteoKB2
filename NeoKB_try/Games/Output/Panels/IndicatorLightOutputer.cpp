#include "IndicatorLightOutputer.h"


using namespace Games::Output::Panels;


int IndicatorLightOutputer::SetupPeripheral(MainInterface * mainInterface)
{
	panel = mainInterface->GetPanel();
	return 0;
}

int IndicatorLightOutputer::pushMessage(IndicatorLightPanelMessage * outputMessage)
{
	if (panel)
		panel->PushOutputMessage(outputMessage);
	else {
		LOG(LogLevel::Error) << "IndicatorLightOutputer::pushMessage : no panel device registered.";
		// throw error
	}
	return 0;
}
