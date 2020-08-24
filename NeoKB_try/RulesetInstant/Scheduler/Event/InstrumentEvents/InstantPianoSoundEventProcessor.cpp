#include "InstantPianoSoundEventProcessor.h"


using namespace Instant::Schedulers::Events::InstrumentEvents;


InstantPianoSoundEventType InstantPianoSoundEventProcessor::GetPianoSoundEventType()
{
	return GetInstrumentEvent()->GetPianoSoundEventType();
}

pair<Pitch, int> InstantPianoSoundEventProcessor::GetSound()
{
	return GetInstrumentEvent()->GetSound();
}

bool InstantPianoSoundEventProcessor::GetPedalDown()
{
	return GetInstrumentEvent()->GetPedalDown();
}
