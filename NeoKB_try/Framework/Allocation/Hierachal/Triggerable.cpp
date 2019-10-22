#include "Triggerable.h"


using namespace Framework::Allocation::Hierachal;


Triggerable::Triggerable(): Schedulable(), RegisterType("Triggerable")
{
}

int Triggerable::SetIsAvailabledForTrigger(bool value)
{
	if (isMaskingTrigger)
		previousIsAvailableForTrigger = value;
	else
		isAvailableForTrigger = value;
	return 0;
}

int Triggerable::MaskTrigger()
{
	if (!isMaskingTrigger) {
		previousIsAvailableForTrigger = isAvailableForTrigger;
		isMaskingTrigger = true;
	}
	isAvailableForTrigger = false;
	return 0;
}

int Triggerable::UnmaskTrigger()
{
	if (isMaskingTrigger) {
		isAvailableForTrigger = previousIsAvailableForTrigger;
		isMaskingTrigger = false;
	}
	return 0;
}

int Triggerable::SetAllChildsIsMaskedForTrigger()
{
	for (int i = 0; i < GetChilds()->size(); i++) {
		dynamic_cast<Triggerable*>(GetChilds()->at(i))->MaskTrigger();
		dynamic_cast<Triggerable*>(GetChilds()->at(i))->SetAllChildsIsMaskedForTrigger();
	}

	return 0;
}

int Triggerable::RecoverAllChildsIsMaskedForTrigger()
{
	for (int i = 0; i < GetChilds()->size(); i++) {
		dynamic_cast<Triggerable*>(GetChilds()->at(i))->UnmaskTrigger();
		dynamic_cast<Triggerable*>(GetChilds()->at(i))->RecoverAllChildsIsMaskedForTrigger();
	}

	return 0;
}

bool Triggerable::GetIsAvailableForTrigger()
{
	return isAvailableForTrigger;
}

bool Triggerable::GetIsInputable()
{
	return isInputable;
}

bool Triggerable::GetIsPresent()
{
	return isPresent;
}

bool Triggerable::GetIsInputReceivable()
{
	return isInputable && isAvailableForTrigger && isPresent;
}

int Triggerable::TriggerOnKeyDown(InputState * inputState, InputKey key)
{
	return onKeyDown(inputState, key);
}

int Triggerable::TriggerOnKeyUp(InputState * inputState, InputKey key)
{
	return onKeyUp(inputState, key);
}

int Triggerable::TriggerOnKnobTurn(InputState * inputState, InputKey knob)
{
	return onKnobTurn(inputState, knob);
}

int Triggerable::TriggerOnButtonDown(InputState * inputState, InputKey button)
{
	return onButtonDown(inputState, button);
}

int Triggerable::TriggerOnButtonUp(InputState * inputState, InputKey button)
{
	return onButtonUp(inputState, button);
}

int Triggerable::TriggerOnSlide(InputState * inputState, InputKey slider)
{
	return onSlide(inputState, slider);
}

int Triggerable::onKeyDown(InputState * inputState, InputKey key)
{
	return -1;
}

int Triggerable::onKeyUp(InputState * inputState, InputKey key)
{
	return -1;
}

int Triggerable::onKnobTurn(InputState * inputState, InputKey knob)
{
	return -1;
}

int Triggerable::onButtonDown(InputState * inputState, InputKey button)
{
	return -1;
}

int Triggerable::onButtonUp(InputState * inputState, InputKey button)
{
	return -1;
}

int Triggerable::onSlide(InputState * inputState, InputKey slider)
{
	return -1;
}



