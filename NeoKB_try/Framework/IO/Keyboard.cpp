#include "Keyboard.h"

using namespace Framework::IO;


template<class _Type>
int Keyboard::AddOnKeyDown(_Type * callableObject, function<int(InputState*)> callback, string name = "HandleKeyDown")
{
	OnKeyDown.Add(callableObject, callback, name);

	return 0;
}