#include "Event.h"

using namespace Base::Schedulers::Events;



Event::Event(const Event* e) {
	startTime = e->startTime;
	lifeTime = e->lifeTime;
}

Event::Event()
{
	throw logic_error("int Event::Event : This constructor is for template constraint. Cannot be executed.");
}

Event::Event(MTO_FLOAT s, MTO_FLOAT l)
{
	startTime = s;

	if (s == -1)
		startType = EventStartType::Immediate;
	else
		startType = EventStartType::Reserved;

	lifeTime = l;
	if (l == -1)
		lifeType = EventLifeType::Infinite;
	else if(l == 0)
		lifeType = EventLifeType::Immediate;
	else
		lifeType = EventLifeType::Timed;
}

Event::Event(const Event &e)
{
	startTime = e.startTime;

	if (startTime == -1)
		startType = EventStartType::Immediate;
	else
		startType = EventStartType::Reserved;

	lifeTime = e.lifeTime;

	if (lifeTime == -1)
		lifeType = EventLifeType::Infinite;
	else if (lifeTime == 0)
		lifeType = EventLifeType::Immediate;
	else
		lifeType = EventLifeType::Timed;

}

EventStartType Event::GetStartType()
{
	return startType;
}

EventLifeType Event::GetLifeType()
{
	return lifeType;
}

int Event::SetStartTime(MTO_FLOAT s)
{
	startTime = s;

	if (startTime == -1)
		startType = EventStartType::Immediate;
	else
		startType = EventStartType::Reserved;

	return 0;
}

int Event::SetLifeTime(MTO_FLOAT l)
{
	lifeTime = l;

	if (lifeTime == -1)
		lifeType = EventLifeType::Infinite;
	else if (lifeTime == 0)
		lifeType = EventLifeType::Immediate;
	else
		lifeType = EventLifeType::Timed;

	return 0;
}

MTO_FLOAT Event::GetStartTime() const
{
	return startTime;
}

MTO_FLOAT Event::GetLifeTime() const
{
	return lifeTime;
}

bool Event::operator<(const Event & rhs) const
{
	return startTime < rhs.GetStartTime();
}

string Event::GetTypeName()
{
	return "Event";
}

Event * Event::Clone()
{
	return new Event(this);
}


