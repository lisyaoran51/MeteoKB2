#include "Panel.h"

#include "../Devices/PanelDevice.h"


using namespace Framework::IO;
using namespace Framework::Devices;


int Panel::SetDevice(Device * device)
{
	Peripheral::SetDevice(device);
	matchedPanelDevice = dynamic_cast<PanelDevice*>(device);
	return 0;
}

int Panel::TriggerOnInput()
{
	if(inputStates.size() > 0)
		LOG(LogLevel::Debug) << "Panel::TriggerOnInput() : trigger on panel event.";

	for (int i = 0; i < inputStates.size(); i++) {
		OnPanelEvent.Trigger(inputStates[i]);
	}
	inputStates.clear();
	return 0;
}

