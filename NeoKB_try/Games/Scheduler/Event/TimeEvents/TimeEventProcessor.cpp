#include "TimeEventProcessor.h"


using namespace Games::Schedulers::Events::TimeEvents;


int TimeEventProcessorInterface::Process()
{
	return ControllTimeController();
}
