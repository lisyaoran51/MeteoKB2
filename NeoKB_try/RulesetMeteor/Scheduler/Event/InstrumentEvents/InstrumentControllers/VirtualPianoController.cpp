#include "VirtualPianoController.h"


using namespace Meteor::Schedulers::Events::InstrumentEvents::InstrumentControllers;


int VirtualPianoController::load()
{
	piano = dynamic_cast<VirtualMeteoPiano*>(instrument);
	return 0;
}

VirtualPianoController::VirtualPianoController() : RegisterType("VirtualPianoController")
{
	registerLoad(bind((int(VirtualPianoController::*)())&VirtualPianoController::load, this));
}

int VirtualPianoController::implementControlInstrument(EventProcessor<Event>* e)
{
	LOG(LogLevel::Debug) << "VirtualPianoController::implementControlInstrument() : Start.";
	if (!piano)
		return -1;

	if (!dynamic_cast<PianoSoundEvent*>(e->GetEvent()))
		return -1;

	PianoSoundEvent* soundEvent = dynamic_cast<PianoSoundEvent*>(e->GetEvent());

	LOG(LogLevel::Debug) << "VirtualPianoController::implementControlInstrument() : play sound [" << soundEvent->GetStartTime() << "].";


	if (soundEvent->GetPianoSoundEventType() == PianoSoundEventType::Pitch) { 
		if (soundEvent->GetSound().second == 0) {
			piano->Stop(soundEvent->GetSound().first);
		}
		else if(soundEvent->GetSound().second > 0){
			piano->Play(soundEvent->GetSound().first, soundEvent->GetSound().second);
		}
	}
	else if (soundEvent->GetPianoSoundEventType() == PianoSoundEventType::Pedal) {
		if (soundEvent->GetPedalDown()) {
			piano->PressPedal();
		}
		else {
			piano->ReleasePedal();
		}
	}

	return 0;
}
