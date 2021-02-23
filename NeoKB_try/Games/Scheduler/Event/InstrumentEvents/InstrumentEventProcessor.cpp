#include "InstrumentEventProcessor.h"


using namespace Games::Schedulers::Events::InstrumentEvents;


int InstrumentEventProcessorInterface::Process()
{
	EventProcessor<Event>::Process();
	return ControlInstrument();
}
