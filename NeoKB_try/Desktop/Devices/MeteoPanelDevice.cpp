#include "MeteoPanelDevice.h"

using namespace Desktop::Devices;


int MeteoPanelDevice::ProcessOutput()
{
	return 0;
}

int MeteoPanelDevice::readFromDevice()
{
	_debugCount++;

	if (_debugCount == 2000) {
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddButton(InputKey::Pause);
		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input.";
	}

	return 0;
}
