#include "SystemEventHandler.h"


using namespace Games::Schedulers::Events::SystemEvents;


int SystemEventHandlerInterface::Process()
{

	EventProcessor<Event>::Process();

	return ControlSystem();
}
