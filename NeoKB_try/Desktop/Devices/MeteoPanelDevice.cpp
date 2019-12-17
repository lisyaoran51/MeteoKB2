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
#if 0
	InputState* newState = meteoPanelBoard->GetPanelState();
	if (newState != nullptr)
		inputStates.push_back(newState);
#else
	_debugCount++;
#endif

	if (_debugCount == -1) {
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddButton(InputKey::Pause);
		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input.";
	}

	if (_debugCount % 1000 == 400) {
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddButton(InputKey::RaiseOctave);
		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input [RaiseOctave].";
	}

	if (_debugCount % 1000 == 900) {
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddButton(InputKey::LowerOctave);
		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input [LowerOctave].";
	}

	if (_debugCount == -1) {
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddKnob(make_pair(InputKey::SectionKnob, 5));
		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake knob input.";
	}





	return 0;
}
