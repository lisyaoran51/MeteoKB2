#include "PanelState.h"

using namespace Framework::Input;



int PanelState::AddButton(pair<InputKey, bool> button)
{
	buttons.push_back(button);
	return 0;
}

vector<pair<InputKey, bool>>* PanelState::GetButtons()
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
	
	if(lastState!= nullptr)
		cloned->SetLastState(lastState);

	cloned->GetButtons()->assign(buttons.begin(), buttons.end());
	cloned->GetKnobs()->assign(knobs.begin(), knobs.end());
	cloned->GetSliders()->assign(sliders.begin(), sliders.end());

	return cloned;
}

bool PanelState::ContainButton(InputKey button)
{
	for (int i = 0; i < buttons.size(); i++)
		if (buttons[i].first == button)
			return true;
	return false;
}

bool PanelState::ContainKnob(InputKey knob)
{
	for (int i = 0; i < knobs.size(); i++)
		if (knobs[i].first == knob)
			return true;
	return false;
}

bool PanelState::ContainSlider(InputKey slider)
{
	for (int i = 0; i < sliders.size(); i++)
		if (sliders[i].first == slider)
			return true;
	return false;
}

bool PanelState::CheckIsEmpty()
{
	return buttons.size() == 0 && knobs.size() == 0 && sliders.size() == 0;
}
