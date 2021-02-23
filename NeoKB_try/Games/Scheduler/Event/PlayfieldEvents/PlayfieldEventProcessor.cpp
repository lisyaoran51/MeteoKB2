#include "PlayfieldEventProcessor.h"


using namespace Games::Schedulers::Events::PlayfieldEvents;


int PlayfieldEventProcessorInterface::Process()
{
	EventProcessor<Event>::Process();
	return ControlPlayfield();
}
