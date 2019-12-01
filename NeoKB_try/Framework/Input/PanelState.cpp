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
	
	if(lastState!= nullptr)
		cloned->SetLastState(lastState);

	cloned->GetButtons()->assign(buttons.begin(), buttons.end());
	cloned->GetKnobs()->assign(knobs.begin(), knobs.end());
	cloned->GetSliders()->assign(sliders.begin(), sliders.end());

	return cloned;
}

bool PanelState::ContainButton(InputKey button)
{
	throw logic_error("PanelState::ContainButton(): not implemented");
	return false;
}

bool PanelState::ContainKnob(InputKey knob)
{
	throw logic_error("PanelState::ContainKnob(): not implemented");
	return false;
}

bool PanelState::ContainSlider(InputKey slider)
{
	throw logic_error("PanelState::ContainerSlider(): not implemented");
	return false;
}

bool PanelState::CheckIsEmpty()
{
	return buttons.size() == 0 && knobs.size() == 0 && sliders.size() == 0;
}
