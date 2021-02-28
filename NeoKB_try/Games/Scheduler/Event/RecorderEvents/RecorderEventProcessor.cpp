#include "RecorderEventProcessor.h"


using namespace Games::Schedulers::Events::RecorderEvents;


int RecorderEventProcessorInterface::Process()
{
	EventProcessor<Event>::Process();
	return AddReplay();
	
}
