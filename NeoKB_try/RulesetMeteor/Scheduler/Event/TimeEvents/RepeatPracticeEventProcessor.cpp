#include "RepeatPracticeEventProcessor.h"


using namespace Meteor::Schedulers::Events::TimeEvents;


RepeatPracticeEventProcessor::RepeatPracticeEventProcessor()
{
}

int RepeatPracticeEventProcessor::GetRepeatSection()
{
	return GetTimeEvent()->GetRepeatSection();
}

float RepeatPracticeEventProcessor::GetRewindLength()
{
	return GetTimeEvent()->GetRewindLength();
}
