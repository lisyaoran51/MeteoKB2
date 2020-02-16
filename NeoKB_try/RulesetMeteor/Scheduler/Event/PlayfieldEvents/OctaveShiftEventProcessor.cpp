#include "OctaveShiftEventProcessor.h"

using namespace Meteor::Schedulers::Events::PlayfieldEvents;


OctaveShiftEventProcessor::OctaveShiftEventProcessor()
{
	playfieldControlType = PlayfieldControlType::Once;
}

OctaveShiftType OctaveShiftEventProcessor::GetShiftType()
{
	if (dynamic_cast<OctaveShiftEvent*>(GetEvent()))
		return dynamic_cast<OctaveShiftEvent*>(GetEvent())->GetShiftType();
	else
		return OctaveShiftType::None;
}

float OctaveShiftEventProcessor::GetShiftStartTime()
{
	return GetStartTime();
}

float OctaveShiftEventProcessor::GetShiftTimeLength()
{
	return GetLifeTime();
}
