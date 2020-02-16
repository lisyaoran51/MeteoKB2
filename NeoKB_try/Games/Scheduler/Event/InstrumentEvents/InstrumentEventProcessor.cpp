#include "InstrumentEventProcessor.h"


using namespace Games::Schedulers::Events::InstrumentEvents;


int InstrumentEventProcessorInterface::Process()
{
	return ControlInstrument();
}
