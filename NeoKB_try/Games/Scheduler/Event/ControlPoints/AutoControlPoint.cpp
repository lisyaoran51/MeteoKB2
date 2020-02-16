#include "AutoControlPoint.h"


using namespace Games::Schedulers::Events::ControlPoints;



AutoControlPoint::AutoControlPoint(MTO_FLOAT s, MTO_FLOAT l): ControlPoint(s, l)
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

int AutoControlPoint::GetSectionIndex()
{
	return sectionIndex;
}

int AutoControlPoint::SetSectionIndex(int sI)
{
	sectionIndex = sI;
	return 0;
}

int AutoControlPoint::GetPartIndex()
{
	return partIndex;
}

int AutoControlPoint::SetPartIndex(int pIndex)
{
	partIndex = pIndex;
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
