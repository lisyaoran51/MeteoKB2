#include "MarkControlPoint.h"


using namespace Games::Schedulers::Events::ControlPoints;


MarkControlPoint::MarkControlPoint(MTO_FLOAT s, MTO_FLOAT l): ControlPoint(s ,l)
{
}

int MarkControlPoint::GetSectionIndex()
{
	return sectionIndex;
}

int MarkControlPoint::SetSectionIndex(int sI)
{
	sectionIndex = sI;
	return 0;
}

int MarkControlPoint::GetPartIndex()
{
	return partIndex;
}

int MarkControlPoint::SetPartIndex(int pIndex)
{
	partIndex = pIndex;
	return 0;
}

string MarkControlPoint::GetTypeName()
{
	return "MarkControlPoint";
}

ControlPointType MarkControlPoint::GetControlPointType()
{
	return ControlPointType::Mark;
}
