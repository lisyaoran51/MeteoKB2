#include "PianoController.h"


using namespace Meteor::Schedulers::Events::InstrumentEvents::InstrumentControllers;


int PianoController::load()
{
	piano = dynamic_cast<Piano*>(instrument);
	return 0;
}

PianoController::PianoController(): InstrumentController<PianoEvent>(), RegisterType("PianoController")
{
	registerLoad(bind((int(PianoController::*)())&PianoController::load, this));
}

int PianoController::implementControlInstrument(EventProcessor<Event>* e)
{
	LOG(LogLevel::Depricated) << "PianoController::implementControlInstrument : make pedal [" << dynamic_cast<PianoEvent*>(e->GetEvent())->GetInput().second << "] on piano [" << piano << "].";

	if (!piano)
		return -1;

	if (!dynamic_cast<PianoEvent*>(e->GetEvent()))
		return -1;

	PianoEvent* pianoEvent = dynamic_cast<PianoEvent*>(e->GetEvent());

	// TODO: 以後要寫輸入琴鍵

	if (pianoEvent->GetInput().first == InputKey::SustainPedal) {
		if (pianoEvent->GetInput().second >= 0) {
			LOG(LogLevel::Depricated) << "PianoController::implementControlInstrument : press down pedal.";
			piano->ControlSustainPedal(true);
		}
		else {
			LOG(LogLevel::Depricated) << "PianoController::implementControlInstrument : release pedal.";
			piano->ControlSustainPedal(false);
		}
	}
	else if (pianoEvent->GetInput().first == InputKey::LowerOctave) {

		piano->MoveOctave(PianoPitchMovement::Lower);
		LOG(LogLevel::Debug) << "PianoController::implementControlInstrument : lower octave.";


	}
	else if (pianoEvent->GetInput().first == InputKey::RaiseOctave) {

		piano->MoveOctave(PianoPitchMovement::Raise);
		LOG(LogLevel::Debug) << "PianoController::implementControlInstrument : raise octave.";
	}

	return 0;
}
