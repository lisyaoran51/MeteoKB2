#include "MeteoPanelDevice.h"

using namespace Desktop::Devices;


MeteoPanelDevice::MeteoPanelDevice(MeteoPanelBoardV1 * panelBoard)
{
	meteoPanelBoard = panelBoard;
}

int MeteoPanelDevice::readFromDevice()
{
#if 1
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

	if (_debugCount % 1000 == -1) {
		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input [RaiseOctave].";
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddButton(InputKey::RaiseOctave);
		inputStates.push_back(newState);

	}

	if (_debugCount % 1000 == -1) {
		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input [LowerOctave].";
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddButton(InputKey::LowerOctave);
		inputStates.push_back(newState);

	}

	if (_debugCount == -1) {
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddKnob(make_pair(InputKey::SectionKnob, 5));
		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake knob input.";
	}

	if (_debugCount % 2000 == -1) {
		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input [SpeedKnob +].";
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddKnob(make_pair(InputKey::SpeedKnob, 5));
		inputStates.push_back(newState);

	}

	if (_debugCount == 0 || _debugCount == -1) {
		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input [SpeedKnob -].";
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddKnob(make_pair(InputKey::SpeedKnob, -10));
		inputStates.push_back(newState);

	}
	_debugCount++;
	return 0;
}

int MeteoPanelDevice::passToDevice()
{

	for (int i = 0; i < outputMessages.size(); i++) {
		LOG(LogLevel::Depricated) << "MeteoPanelDevice::passToDevice() : pass message to board.";
		meteoPanelBoard->PushI2cMessage(outputMessages[i]->ToString());
		delete outputMessages[i];
	}
	outputMessages.clear();

	return 0;
}
