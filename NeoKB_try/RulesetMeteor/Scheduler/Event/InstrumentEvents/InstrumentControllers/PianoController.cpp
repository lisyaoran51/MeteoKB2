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
	if (!piano)
		return -1;

	if (!dynamic_cast<PianoEvent*>(e))
		return -1;

	PianoEvent* pianoEvent = dynamic_cast<PianoEvent*>(e);

	// TODO: 以後要寫輸入琴鍵

	if (pianoEvent->GetInput().first == InputKey::SustainPedal) {
		if (pianoEvent->GetInput().second >= 0) {
			piano->ControlSustainPedal(true);
		}
		else {
			piano->ControlSustainPedal(false);
		}
	}

	return 0;
}
