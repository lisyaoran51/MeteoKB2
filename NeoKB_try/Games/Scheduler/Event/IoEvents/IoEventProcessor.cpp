#include "IoEventProcessor.h"


using namespace Games::Schedulers::Events::IoEvents;


int IoEventProcessorInterface::Process()
{
	return ProcessIo();
}
