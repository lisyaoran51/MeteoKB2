#include "InstantVirtualPianoController.h"

#include "../../../../../Instruments/CompositeMeteoPiano.h"


using namespace Instant::Schedulers::Events::InstrumentEvents::InstrumentControllers;
using namespace Instruments;


int InstantVirtualPianoController::load()
{
	piano = dynamic_cast<CompositeMeteoPiano*>(instrument)->GetVirtualMeteoPiano();
	return 0;
}

InstantVirtualPianoController::InstantVirtualPianoController() : RegisterType("InstantVirtualPianoController")
{
	registerLoad(bind((int(InstantVirtualPianoController::*)())&InstantVirtualPianoController::load, this));
}

int InstantVirtualPianoController::implementControlInstrument(EventProcessor<Event>* e)
{
	LOG(LogLevel::Depricated) << "InstantVirtualPianoController::implementControlInstrument() : Start.";
	if (!piano)
		return -1;

	if (!dynamic_cast<InstantPianoSoundEvent*>(e->GetEvent()))
		return -1;

	InstantPianoSoundEvent* soundEvent = dynamic_cast<InstantPianoSoundEvent*>(e->GetEvent());



	if (soundEvent->GetPianoSoundEventType() == InstantPianoSoundEventType::Pitch) {
		LOG(LogLevel::Debug) << "InstantVirtualPianoController::implementControlInstrument() : play sound [" << soundEvent->GetStartTime() << "] on [" << int(soundEvent->GetSound().first) << "] with volume [" << soundEvent->GetSound().second << "].";
		if (soundEvent->GetSound().second == 0) {
			piano->Stop(soundEvent->GetSound().first);
		}
		else if (soundEvent->GetSound().second > 0) {
			piano->Play(soundEvent->GetSound().first, soundEvent->GetSound().second);
		}
	}
	else if (soundEvent->GetPianoSoundEventType() == InstantPianoSoundEventType::Pedal) {
		LOG(LogLevel::Depricated) << "InstantVirtualPianoController::implementControlInstrument() : pedal [" << soundEvent->GetStartTime() << "] " << (soundEvent->GetPedalDown() ? "down" : "up") << ".";
		if (soundEvent->GetPedalDown()) {
			piano->PressPedal();
		}
		else {
			piano->ReleasePedal();
		}
	}
	else if (soundEvent->GetPianoSoundEventType() == InstantPianoSoundEventType::Octave) {
		LOG(LogLevel::Depricated) << "InstantVirtualPianoController::implementControlInstrument() : octave [" << soundEvent->GetStartTime() << "] " << (soundEvent->GetOctaveMovement() == PianoPitchMovement::Lower ? "Lower" : "Raise") << ".";
		
		if (soundEvent->GetOctaveMovement() == PianoPitchMovement::Lower) {
			piano->MoveOctave(PianoPitchMovement::Lower);
		}
		else if (soundEvent->GetOctaveMovement() == PianoPitchMovement::Raise) {
			piano->MoveOctave(PianoPitchMovement::Raise);
		}
		else
			LOG(LogLevel::Warning) << "InstantVirtualPianoController::implementControlInstrument() : wrong octave.";
	}

	return 0;
}

int InstantVirtualPianoController::implementFastForwardControlInstrument(EventProcessor<Event>* e)
{
	LOG(LogLevel::Error) << "InstantVirtualPianoController::implementFastForwardControlInstrument() : no this function.";
	return 0;
}

int InstantVirtualPianoController::implementUndoControlInstrument(EventProcessor<Event>* e)
{
	LOG(LogLevel::Error) << "InstantVirtualPianoController::implementUndoControlInstrument() : no this function.";
	return 0;
}
