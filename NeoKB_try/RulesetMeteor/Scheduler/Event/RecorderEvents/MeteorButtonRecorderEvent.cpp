#include "MeteorButtonRecorderEvent.h"


using namespace Meteor::Schedulers::Events::RecorderEvents;


MeteorButtonRecorderEvent::MeteorButtonRecorderEvent(pair<InputKey, int> c, MTO_FLOAT s, MTO_FLOAT l) : RecorderEvent(s, l)
{
	command = c;
}

pair<InputKey, int> MeteorButtonRecorderEvent::GetInput()
{
	return command;
}

string MeteorButtonRecorderEvent::GetTypeName()
{
	return "MeteorButtonRecorderEvent";
}
