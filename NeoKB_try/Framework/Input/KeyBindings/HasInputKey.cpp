#include "HasInputKey.h"


using namespace Framework::Input::KeyBindings;


HasInputKey::HasInputKey(InputKey iKey, int v): inputKey(iKey), inputValue(v)
{
}

InputKey HasInputKey::GetInputKey()
{
	return inputKey;
}

int HasInputKey::GetInputValue()
{
	return inputValue;
}
