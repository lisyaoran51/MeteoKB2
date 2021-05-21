#include "VirtualPianoController.h"

#include "../../../../../Instruments/CompositeMeteoPiano.h"


// debug¥Î
#include <chrono>
using namespace std::chrono;
// debug¥Î

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
		LOG(LogLevel::Fine) << "VirtualPianoController::implementControlInstrument() : play sound [" << soundEvent->GetStartTime() << "] on [" << int(soundEvent->GetSound().first) << "] with volume [" << soundEvent->GetSound().second << "].";
		if (soundEvent->GetSound().second == 0) {
			piano->Stop(soundEvent->GetSound().first);
		}
		else if(soundEvent->GetSound().second > 0){
			system_clock::time_point systemStartTime = system_clock::now();

			piano->Play(soundEvent->GetSound().first, soundEvent->GetSound().second);

			system_clock::time_point systemCurrentTime = system_clock::now();
			LOG(LogLevel::Fine) << "VirtualPianoController::implementControlInstrument() : play sound cost time = [" << duration_cast<microseconds>(systemCurrentTime - systemStartTime).count() << "].";
			system_clock::time_point newSystemCurrentTime = system_clock::now();
			LOG(LogLevel::Fine) << "VirtualPianoController::implementControlInstrument() : log print time = [" << duration_cast<microseconds>(newSystemCurrentTime - systemCurrentTime).count() << "].";

		}
	}
	else if (soundEvent->GetPianoSoundEventType() == PianoSoundEventType::Pedal) {
		LOG(LogLevel::Depricated) << "VirtualPianoController::implementControlInstrument() : pedal [" << soundEvent->GetStartTime() << "] " << (soundEvent->GetPedalDown() ? "down" : "up") << ".";
		if (soundEvent->GetPedalDown()) {
			piano->PressPedal();
		}
		else {
			piano->ReleasePedal();
		}
	}

	return 0;
}

int VirtualPianoController::implementFastForwardControlInstrument(EventProcessor<Event>* e)
{
	LOG(LogLevel::Error) << "VirtualPianoController::implementFastForwardControlInstrument() : no this function.";
	return 0;
}

int VirtualPianoController::implementUndoControlInstrument(EventProcessor<Event>* e)
{
	LOG(LogLevel::Error) << "VirtualPianoController::implementUndoControlInstrument() : no this function.";
	return 0;
}
