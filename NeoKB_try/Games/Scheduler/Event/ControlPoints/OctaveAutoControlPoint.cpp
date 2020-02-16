#include "OctaveAutoControlPoint.h"


using namespace Games::Schedulers::Events::ControlPoints;



OctaveAutoControlPoint::OctaveAutoControlPoint(InputKey iKey, MTO_FLOAT s, MTO_FLOAT l): AutoControlPoint(s, l), HasInputKey(iKey, 1)
{
}

OctaveAutoControlPoint::OctaveAutoControlPoint(InputKey iKey, int iValue, MTO_FLOAT s, MTO_FLOAT l) : AutoControlPoint(s, l), HasInputKey(iKey, iValue)
{
}

ControlPointType OctaveAutoControlPoint::GetControlPointType()
{
	return ControlPointType::Effect;
}

string OctaveAutoControlPoint::GetTypeName()
{
	return "OctaveAutoControlPoint";
}

Event * OctaveAutoControlPoint::Clone()
{
	OctaveAutoControlPoint* newOctaveAutoControlPoint = new OctaveAutoControlPoint(inputKey, inputValue, startTime, lifeTime);
	newOctaveAutoControlPoint->SetVolume(volume);
	newOctaveAutoControlPoint->SetSectionIndex(sectionIndex);
	newOctaveAutoControlPoint->SetHandType(handType);
	newOctaveAutoControlPoint->SetPartIndex(partIndex);

	return newOctaveAutoControlPoint;
}
