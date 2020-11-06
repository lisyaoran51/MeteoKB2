#include "SectionStartControlPoint.h"


using namespace Games::Schedulers::Events::ControlPoints;


SectionStartControlPoint::SectionStartControlPoint(int sIndex, MTO_FLOAT s, MTO_FLOAT l) : MarkControlPoint(s, l)
{
	sectionIndex = sIndex;
}
