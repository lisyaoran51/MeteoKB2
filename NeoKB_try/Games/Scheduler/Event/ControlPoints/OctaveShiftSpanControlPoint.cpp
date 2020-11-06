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
