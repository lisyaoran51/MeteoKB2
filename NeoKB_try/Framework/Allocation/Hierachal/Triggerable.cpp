#include "Triggerable.h"


using namespace Framework::Allocation::Hierachal;


Triggerable::Triggerable(): Schedulable(), RegisterType("Triggerable")
{
}

int Triggerable::TriggerOnKeyDown(InputState * inputState, Key key)
{
	return onKeyDown(inputState, key);
}

int Triggerable::TriggerOnKeyUp(InputState * inputState, Key key)
{
	return onKeyUp(inputState, key);
}

int Triggerable::TriggerOnKnobTurn(InputState * inputState, PanelKnob knob)
{
	return onKnobTurn(inputState, knob);
}

int Triggerable::TriggerOnButtonDown(InputState * inputState, PanelButton button)
{
	return onButtonDown(inputState, button);
}

int Triggerable::TriggerOnButtonUp(InputState * inputState, PanelButton button)
{
	return onButtonUp(inputState, button);
}

int Triggerable::TriggerOnSlide(InputState * inputState, PanelSlider slider)
{
	return onSlide(inputState, slider);
}

int Triggerable::onKeyDown(InputState * inputState, Key key)
{
	return 0;
}

int Triggerable::onKeyUp(InputState * inputState, Key key)
{
	return 0;
}

int Triggerable::onKnobTurn(InputState * inputState, PanelKnob knob)
{
	return 0;
}

int Triggerable::onButtonDown(InputState * inputState, PanelButton button)
{
	return 0;
}

int Triggerable::onButtonUp(InputState * inputState, PanelButton button)
{
	return 0;
}

int Triggerable::onSlide(InputState * inputState, PanelSlider slider)
{
	return 0;
}



