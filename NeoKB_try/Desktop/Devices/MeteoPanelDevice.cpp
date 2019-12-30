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

	if (_debugCount % 2000 == 1000) {
		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input [SpeedKnob +].";
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddKnob(make_pair(InputKey::SpeedKnob, 5));
		inputStates.push_back(newState);

	}

	if (_debugCount % 2000 == -1) {
		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input [SpeedKnob -].";
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddKnob(make_pair(InputKey::SpeedKnob, -5));
		inputStates.push_back(newState);

	}

	return 0;
}

int MeteoPanelDevice::passToDevice()
{
	vector<OutputMessage*> messages;
	matchedPeripheral->PourOutOutputMessages(&messages);

	for (int i = 0; i < messages.size(); i++) {
		meteoPanelBoard->PushI2cMessage(messages[i]->ToString());
		delete messages[i];
	}

	return 0;
}
