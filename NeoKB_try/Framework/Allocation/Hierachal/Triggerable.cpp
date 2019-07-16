#include "Triggerable.h"


using namespace Framework::Allocation::Hierachal;


Triggerable::Triggerable(): Updatable(), RegisterType("Triggerable")
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

int Triggerable::TriggerOnKnobTurn(InputState * inputState, Knob knob)
{
	return onKnobTurn(inputState, knob);
}

int Triggerable::TriggerOnButtonDown(InputState * inputState, Button button)
{
	return onButtonDown(inputState, button);
}

int Triggerable::TriggerOnButtonUp(InputState * inputState, Button button)
{
	return onButtonUp(inputState, button);
}

int Triggerable::TriggerOnSlide(InputState * inputState, Slider slider)
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

int Triggerable::onKnobTurn(InputState * inputState, Knob knob)
{
	return 0;
}

int Triggerable::onButtonDown(InputState * inputState, Button button)
{
	return 0;
}

int Triggerable::onButtonUp(InputState * inputState, Button button)
{
	return 0;
}

int Triggerable::onSlide(InputState * inputState, Slider slider)
{
	return 0;
}



