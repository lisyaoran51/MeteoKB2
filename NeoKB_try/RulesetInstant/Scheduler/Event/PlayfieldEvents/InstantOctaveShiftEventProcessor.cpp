#include "InstantOctaveShiftEventProcessor.h"

using namespace Instant::Schedulers::Events::PlayfieldEvents;


InstantOctaveShiftEventProcessor::InstantOctaveShiftEventProcessor()
{
	playfieldControlType = PlayfieldControlType::Once;
}

InstantOctaveShiftType InstantOctaveShiftEventProcessor::GetShiftType()
{
	if (dynamic_cast<InstantOctaveShiftEvent*>(GetEvent()))
		return dynamic_cast<InstantOctaveShiftEvent*>(GetEvent())->GetShiftType();
	else
		return InstantOctaveShiftType::None;
}

float InstantOctaveShiftEventProcessor::GetShiftStartTime()
{
	return GetStartTime();
}
