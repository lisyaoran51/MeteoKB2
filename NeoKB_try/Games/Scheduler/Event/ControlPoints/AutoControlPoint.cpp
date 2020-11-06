#include "AutoControlPoint.h"


using namespace Games::Schedulers::Events::ControlPoints;



AutoControlPoint::AutoControlPoint(MTO_FLOAT s, MTO_FLOAT l): MarkControlPoint(s, l)
{
}

MTO_FLOAT AutoControlPoint::GetVolume()
{
	return volume;
}

int AutoControlPoint::SetVolume(MTO_FLOAT v)
{
	volume = v;
	return 0;
}

HandType AutoControlPoint::GetHandType()
{
	return handType;
}

int AutoControlPoint::SetHandType(HandType h)
{
	handType = h;
	return 0;
}

string AutoControlPoint::GetTypeName()
{
	return "AutoControlPoint";
}
