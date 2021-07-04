#include "InstantPianoController.h"

#include "../../../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"


using namespace Instant::Schedulers::Events::InstrumentEvents::InstrumentControllers;
using namespace Games::Output::Bluetooths;


int InstantPianoController::load()
{
	piano = dynamic_cast<Piano*>(instrument);

	OutputManager * o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("PianoController::load() : OutputManager not found in cache.");


	return load(o);
}

int InstantPianoController::load(OutputManager * o)
{
	outputManager = o;
	return 0;
}

InstantPianoController::InstantPianoController() : InstrumentController<InstantPianoEvent>(), RegisterType("InstantPianoController")
{
	registerLoad(bind((int(InstantPianoController::*)())&InstantPianoController::load, this));
}

int InstantPianoController::implementControlInstrument(EventProcessor<Event>* e)
{
	LOG(LogLevel::Depricated) << "InstantPianoController::implementControlInstrument : make pedal [" << dynamic_cast<InstantPianoEvent*>(e->GetEvent())->GetInput().second << "] on piano [" << piano << "].";

	if (!piano)
		return -1;

	if (!dynamic_cast<InstantPianoEvent*>(e->GetEvent()))
		return -1;

	InstantPianoEvent* pianoEvent = dynamic_cast<InstantPianoEvent*>(e->GetEvent());

	// TODO: 以後要寫輸入琴鍵

	if (pianoEvent->GetInput().first == InputKey::SustainPedal) {


		if (pianoEvent->GetInput().second >= 0) {
			LOG(LogLevel::Depricated) << "InstantPianoController::implementControlInstrument : press down pedal.";
			piano->ControlSustainPedal(true);
		}
		else {
			LOG(LogLevel::Depricated) << "InstantPianoController::implementControlInstrument : release pedal.";
			piano->ControlSustainPedal(false);
		}

	}
	else if (pianoEvent->GetInput().first == InputKey::LowerOctave) {

		piano->MoveOctave(PianoPitchMovement::Lower);
		LOG(LogLevel::Debug) << "InstantPianoController::implementControlInstrument : lower octave.";

	}
	else if (pianoEvent->GetInput().first == InputKey::RaiseOctave) {

		piano->MoveOctave(PianoPitchMovement::Raise);
		LOG(LogLevel::Debug) << "InstantPianoController::implementControlInstrument : raise octave.";
	}

	return 0;
}

int InstantPianoController::implementFastForwardControlInstrument(EventProcessor<Event>* e)
{
	return 0;
}

int InstantPianoController::implementUndoControlInstrument(EventProcessor<Event>* e)
{
	return 0;
}
