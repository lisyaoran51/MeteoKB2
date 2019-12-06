#include "MeteoPanelDevice.h"

using namespace Desktop::Devices;


MeteoPanelDevice::MeteoPanelDevice(MeteoPanelBoardV1 * panelBoard)
{
	meteoPanelBoard = panelBoard;
}

int MeteoPanelDevice::ProcessOutput()
{
	return 0;
}

int MeteoPanelDevice::readFromDevice()
{

	InputState* newState = meteoPanelBoard->GetPanelState();
	if (newState != nullptr)
		inputStates.push_back(newState);

	_debugCount++;

	if (_debugCount == 4000) {
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddButton(InputKey::Pause);
		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input.";
	}

	if (_debugCount == 5000) {
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddButton(InputKey::Pause);
		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input.";
	}

	if (_debugCount == 8000) {
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddKnob(make_pair(InputKey::SectionKnob, 5));
		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake knob input.";
	}




	return 0;
}
