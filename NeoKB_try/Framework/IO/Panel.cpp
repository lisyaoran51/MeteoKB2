#include "Panel.h"

using namespace  Framework::IO;

int Panel::SetDevice(Device * device)
{
	Peripheral::SetDevice(device);
	matchedPanelDevice = dynamic_cast<PanelDevice*>(device);
	return 0;
}

int Panel::TriggerOnInput()
{
	for (int i = 0; i < inputStates.size(); i++) {
		OnPanelEvent.Trigger(inputStates[i]);
	}
	inputStates.clear();
	return 0;
}


template<class _Type>
int Panel::AddOnPanelEvent(_Type * callableObject, function<int(InputState*)> callback, string name = "HandlePanelEvent")
{
	OnPanelEvent.Add(callableObject, callback, name);
	return 0;
}