#include "PanelState.h"

using namespace Framework::Input;



int PanelState::AddButton(InputKey button)
{
	buttons.push_back(button);
	return 0;
}

vector<InputKey>* PanelState::GetButtons()
{
	return &buttons;
}

int PanelState::AddKnob(pair<InputKey, int> knob)
{
	knobs.push_back(knob);
	return 0;
}

vector<pair<InputKey, int>>* PanelState::GetKnobs()
{
	return &knobs;
}

int PanelState::AddSlider(pair<InputKey, int> slider)
{
	sliders.push_back(slider);
	return 0;
}

vector<pair<InputKey, int>>* PanelState::GetSliders()
{
	return &sliders;
}

PanelState * PanelState::Clone()
{
	PanelState* cloned = new PanelState();
	
	cloned->SetLastState(lastState);

	cloned->GetButtons()->assign(buttons.begin(), buttons.end());
	cloned->GetKnobs()->assign(knobs.begin(), knobs.end());
	cloned->GetSliders()->assign(sliders.begin(), sliders.end());

	return cloned;
}
