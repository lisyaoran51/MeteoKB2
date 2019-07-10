#include "Panel.h"

using namespace  Framework::IO;

template<class _Type>
int Panel::AddOnButtonDown(_Type * callableObject, function<int(InputState*)> callback, string name = "HandleButtonDown")
{
	OnButtonDown.Add(callableObject, callback, name);
	return 0;
}

template<class _Type>
int Panel::AddOnKnobTurn(_Type * callableObject, function<int(InputState*)> callback, string name = "HandleKnobTurn")
{
	OnKnobTurn.Add(callableObject, callback, name);
	return 0;
}

template<class _Type>
int Panel::AddOnSliderMove(_Type * callableObject, function<int(InputState*)> callback, string name = "HandleSliderMove")
{
	OnSliderMove(callableObject, callback, name);
	return 0;
}
