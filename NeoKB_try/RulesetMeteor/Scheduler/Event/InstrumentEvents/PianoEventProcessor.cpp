#include "PianoEventProcessor.h"


using namespace Meteor::Schedulers::Events::InstrumentEvents;


pair<InputKey, int> PianoEventProcessor::GetInput()
{
	return GetInstrumentEvent()->GetInput();
}
