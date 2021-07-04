#include "InstantPianoEventProcessor.h"


using namespace Instant::Schedulers::Events::InstrumentEvents;


pair<InputKey, int> InstantPianoEventProcessor::GetInput()
{
	return GetInstrumentEvent()->GetInput();
}
