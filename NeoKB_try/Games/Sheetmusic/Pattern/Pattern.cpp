#include "Pattern.h"

using namespace Games::Sheetmusics::Patterns;



Pattern::Pattern()
{
}

Pattern::Pattern(Event * oEvent) : originalEvent(oEvent)
{
}

int Pattern::Add(Event * e)
{
	events.push_back(e);
	return 0;
}

vector<Event*>* Pattern::GetEvents()
{
	return &events;
}

int Pattern::SetOriginalEvent(Event * e)
{
	originalEvent = e;
	return 0;
}

Event * Pattern::GetOriginalEvent()
{
	return originalEvent;
}
