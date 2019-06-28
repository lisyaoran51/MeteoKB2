#include "Triggerable.h"


using namespace Framework::Allocation::Hierachal;


Triggerable::Triggerable(): Updatable(), RegisterType("Triggerable")
{
}

int Triggerable::TriggerOnKeyDown(InputState * inputState)
{
	return onKeyDown(inputState);
}

int Triggerable::onKeyDown(InputState * inputState)
{
	return 0;
}
