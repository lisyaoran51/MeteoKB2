#include "RepeatPracticeEvent.h"


using namespace Meteor::Schedulers::Events::TimeEvents;


RepeatPracticeEvent::RepeatPracticeEvent(int section, float rLength, MTO_FLOAT s, MTO_FLOAT l) : TimeEvent(s, l)
{
	repeatSection = section;
	rewindLength = rLength;
}

string RepeatPracticeEvent::GetTypeName()
{
	return "RepeatPracticeEvent";
}

int RepeatPracticeEvent::GetRepeatSection()
{
	return repeatSection;
}

float RepeatPracticeEvent::GetRewindLength()
{
	return rewindLength;
}

Event * RepeatPracticeEvent::Clone()
{
	RepeatPracticeEvent* newEvent = new RepeatPracticeEvent(repeatSection, rewindLength, startTime, lifeTime);
	newEvent->SetSourceEvent(sourceEvent);

	return newEvent;
}
