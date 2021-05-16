#include "OctaveShiftSpanControlPoint.h"


using namespace Games::Schedulers::Events::ControlPoints;


OctaveShiftSpanControlPoint::OctaveShiftSpanControlPoint(OctaveRange oRange, MTO_FLOAT s, MTO_FLOAT l): MarkControlPoint(s, l)
{
	octaveRange = oRange;
}

int OctaveShiftSpanControlPoint::SetOctaveRange(OctaveRange oRange)
{
	octaveRange = oRange;
	return 0;
}

OctaveRange OctaveShiftSpanControlPoint::GetOctaveRange()
{
	return octaveRange;
}

string OctaveShiftSpanControlPoint::GetTypeName()
{
	return "OctaveShiftSpanControlPoint";
}

Event * OctaveShiftSpanControlPoint::Clone()
{
	OctaveShiftSpanControlPoint* newEvent = new OctaveShiftSpanControlPoint(octaveRange, startTime, lifeTime);
	newEvent->SetSectionIndex(sectionIndex);
	newEvent->SetPartIndex(partIndex);
	return newEvent;
}
