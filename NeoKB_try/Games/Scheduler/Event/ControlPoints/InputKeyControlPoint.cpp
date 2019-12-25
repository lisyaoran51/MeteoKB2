#include "InputKeyControlPoint.h"



using namespace Games::Schedulers::Events::ControlPoints;


InputKeyControlPoint::InputKeyControlPoint(InputKey iKey, MTO_FLOAT s, MTO_FLOAT l): PlayableControlPoint(s, l), HasInputKey(iKey, 0)
{
}

InputKeyControlPoint::InputKeyControlPoint(InputKey iKey, int iValue, MTO_FLOAT s, MTO_FLOAT l) : PlayableControlPoint(s, l), HasInputKey(iKey, iValue)
{
}

ControlPointType InputKeyControlPoint::GetControlPointType()
{
	return ControlPointType::Input;
}

string InputKeyControlPoint::GetTypeName()
{
	return string("InputKeyControlPoint");
}

Event * InputKeyControlPoint::Clone()
{
	InputKeyControlPoint* newInputKeyControlPoint = new InputKeyControlPoint(inputKey, inputValue, startTime, lifeTime);
	newInputKeyControlPoint->SetVolume(volume);
	newInputKeyControlPoint->SetSectionIndex(sectionIndex);
	newInputKeyControlPoint->SetHandType(handType);
	newInputKeyControlPoint->SetPartIndex(partIndex);

	return newInputKeyControlPoint;
}
