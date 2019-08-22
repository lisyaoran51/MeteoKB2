#include "PanelState.h"

using namespace Framework::Input;



int PanelState::AddButton(PanelButton button)
{
	buttons.push_back(button);
	return 0;
}

vector<PanelButton>* PanelState::GetButtons()
{
	return &buttons;
}

int PanelState::AddKnob(pair<PanelKnob, int> knob)
{
	knobs.push_back(knob);
	return 0;
}

vector<pair<PanelKnob, int>>* PanelState::Getknobs()
{
	return &knobs;
}

int PanelState::AddSlider(pair<PanelSlider, int> slider)
{
	sliders.push_back(slider);
	return 0;
}

vector<pair<PanelSlider, int>>* PanelState::GetSliders()
{
	return &sliders;
}

PanelState * PanelState::Clone()
{
	PanelState* cloned = new PanelState();
	
	cloned->SetLastState(lastState);

	cloned->GetButtons()->assign(buttons.begin(), buttons.end());
	cloned->Getknobs()->assign(knobs.begin(), knobs.end());
	cloned->GetSliders()->assign(sliders.begin(), sliders.end());

	return cloned;
}
