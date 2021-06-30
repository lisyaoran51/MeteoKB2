#include "VirtualPianoController.h"

#include "../../../../../Instruments/CompositeMeteoPiano.h"
#include "../../../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include <iomanip>
#include <sstream>


// debug¥Î
#include <chrono>
using namespace std::chrono;
// debug¥Î

using namespace Meteor::Schedulers::Events::InstrumentEvents::InstrumentControllers;
using namespace Instruments;
using namespace Games::Output::Bluetooths;


int VirtualPianoController::load()
{
	piano = dynamic_cast<CompositeMeteoPiano*>(instrument)->GetVirtualMeteoPiano();

	OutputManager * o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("PianoController::load() : OutputManager not found in cache.");


	return load(o);
}

int VirtualPianoController::load(OutputManager * o)
{
	outputManager = o;
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
		
		string gameEventContext;

		if (soundEvent->GetSound().second == 0) {
			piano->Stop(soundEvent->GetSound().first);
			gameEventContext = string("GameStopSound,") + to_string(int(soundEvent->GetSound().first));
		}
		else if(soundEvent->GetSound().second > 0){

			piano->Play(soundEvent->GetSound().first, soundEvent->GetSound().second);

			gameEventContext = string("GamePlaySound,") + to_string(int(soundEvent->GetSound().first)) + string(",");

			stringstream stream;
			stream << fixed << setprecision(2) << soundEvent->GetSound().second;
			gameEventContext += stream.str();
		}

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::HardwareGameEvent);

		json context;
		context["Events"].push_back(gameEventContext);

		meteoContextBluetoothMessage->SetContextInJson(context);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		if (outputManager != nullptr)
			outputManager->PushMessage(meteoContextBluetoothMessage);

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
	//LOG(LogLevel::Error) << "VirtualPianoController::implementFastForwardControlInstrument() : no this function.";
	return 0;
}

int VirtualPianoController::implementUndoControlInstrument(EventProcessor<Event>* e)
{
	//LOG(LogLevel::Error) << "VirtualPianoController::implementUndoControlInstrument() : no this function.";
	return 0;
}
