#include "MeteoPanelDevice.h"

using namespace Desktop::Devices;


MeteoPanelDevice::MeteoPanelDevice(MeteoMcuV1 * mMcu)
{
	meteoMcu = mMcu;
}

int MeteoPanelDevice::readFromDevice()
{
#if 1
	InputState* newState = meteoMcu->GetPanelState();
	if (newState != nullptr)
		inputStates.push_back(newState);
#else
	_debugCount++;
#endif

#if 0
	_debugCount++;
#endif

	if (_debugCount == -1) {
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddButton(pair<InputKey, int>(InputKey::Pause, true));
		inputStates.push_back(newState);

		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input.";
	}

	if (_debugCount % 1000 == -1) {
		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input [RaiseOctave].";
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddButton(pair<InputKey, int>(InputKey::RaiseOctave, true));
		inputStates.push_back(newState);

	}

	if (_debugCount % 1000 == -1) {
		LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Create fake pause input [LowerOctave].";
		InputState* newState = new InputState();
		newState->SetPanelState(new PanelState());

		newState->GetPanelState()->AddButton(pair<InputKey, int>(InputKey::LowerOctave, true));
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

	if (_debugCount % 1000 == 1) {
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
	bool test = false;
	unique_lock<mutex> uLock(outputMessageMutex);
	for (int i = 0; i < outputMessages.size(); i++) {
		test = true;
		LOG(LogLevel::Debug) << "MeteoPanelDevice::passToDevice() : pass message to board.";
		meteoMcu->PushI2cMessage(outputMessages[i]->ToString());
		delete outputMessages[i];
		LOG(LogLevel::Debug) << "MeteoPanelDevice::passToDevice() : pass over.";
	}
	outputMessages.push_back(nullptr);
	outputMessages.clear();
	if(test)
		LOG(LogLevel::Debug) << "MeteoPanelDevice::passToDevice() : clear.";
	meteoMcu->PushI2cMessage("SK,0");

	return 0;
}
