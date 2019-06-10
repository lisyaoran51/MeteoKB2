#include "ControlPoint.h"

using namespace Base::Schedulers::Events::ControlPoints;


ControlPoint::ControlPoint(MTO_FLOAT s, MTO_FLOAT l): Event(s,l)
{
}

string ControlPoint::GetTypeName()
{
	return "ControlPoint";
}
