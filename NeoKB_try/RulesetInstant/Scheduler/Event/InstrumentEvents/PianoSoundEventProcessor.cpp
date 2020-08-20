#include "PianoSoundEventProcessor.h"


using namespace Meteor::Schedulers::Events::InstrumentEvents;


PianoSoundEventType PianoSoundEventProcessor::GetPianoSoundEventType()
{
	return GetInstrumentEvent()->GetPianoSoundEventType();
}

pair<Pitch, int> PianoSoundEventProcessor::GetSound()
{
	return GetInstrumentEvent()->GetSound();
}

bool PianoSoundEventProcessor::GetPedalDown()
{
	return GetInstrumentEvent()->GetPedalDown();
}
