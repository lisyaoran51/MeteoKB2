#include "VirtualPianoController.h"

#include "../../../../../Instruments/CompositeMeteoPiano.h"


using namespace Meteor::Schedulers::Events::InstrumentEvents::InstrumentControllers;
using namespace Instruments;


int VirtualPianoController::load()
{
	piano = dynamic_cast<CompositeMeteoPiano*>(instrument)->GetVirtualMeteoPiano();
	return 0;
}

VirtualPianoController::VirtualPianoController() : RegisterType("VirtualPianoController")
{
	registerLoad(bind((int(VirtualPianoController::*)())&VirtualPianoController::load, this));
}

int VirtualPianoController::implementControlInstrument(EventProcessor<Event>* e)
{
	LOG(LogLevel::Depricated) << "VirtualPianoController::implementControlInstrument() : Start.";
	if (!piano)
		return -1;

	if (!dynamic_cast<PianoSoundEvent*>(e->GetEvent()))
		return -1;

	PianoSoundEvent* soundEvent = dynamic_cast<PianoSoundEvent*>(e->GetEvent());



	if (soundEvent->GetPianoSoundEventType() == PianoSoundEventType::Pitch) {
		LOG(LogLevel::Debug) << "VirtualPianoController::implementControlInstrument() : play sound [" << soundEvent->GetStartTime() << "] on [" << int(soundEvent->GetSound().first) << "] with volume [" << soundEvent->GetSound().second << "].";
		if (soundEvent->GetSound().second == 0) {
			piano->Stop(soundEvent->GetSound().first);
		}
		else if(soundEvent->GetSound().second > 0){
			piano->Play(soundEvent->GetSound().first, soundEvent->GetSound().second);
		}
	}
	else if (soundEvent->GetPianoSoundEventType() == PianoSoundEventType::Pedal) {
		LOG(LogLevel::Debug) << "VirtualPianoController::implementControlInstrument() : pedal [" << soundEvent->GetStartTime() << "] " << (soundEvent->GetPedalDown() ? "down" : "up") << ".";
		if (soundEvent->GetPedalDown()) {
			piano->PressPedal();
		}
		else {
			piano->ReleasePedal();
		}
	}

	return 0;
}
