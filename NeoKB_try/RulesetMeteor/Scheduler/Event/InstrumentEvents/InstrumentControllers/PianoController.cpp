#include "PianoController.h"

#include "../../../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"


using namespace Meteor::Schedulers::Events::InstrumentEvents::InstrumentControllers;
using namespace Games::Output::Bluetooths;


int PianoController::load()
{
	piano = dynamic_cast<Piano*>(instrument);

	OutputManager * o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("PianoController::load() : OutputManager not found in cache.");


	return load(o);
}

int PianoController::load(OutputManager * o)
{
	outputManager = o;
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


		string gameEventContext;

		if (pianoEvent->GetInput().second >= 0) {
			LOG(LogLevel::Depricated) << "PianoController::implementControlInstrument : press down pedal.";
			piano->ControlSustainPedal(true);
			gameEventContext = "GameControlPedalDown";
		}
		else {
			LOG(LogLevel::Depricated) << "PianoController::implementControlInstrument : release pedal.";
			piano->ControlSustainPedal(false);
			gameEventContext = "GameControlPedalUp";
		}

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::HardwareGameEvent);

		json context;
		context["Events"].push_back(gameEventContext);

		meteoContextBluetoothMessage->SetContextInJson(context);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		if (outputManager != nullptr)
			outputManager->PushMessage(meteoContextBluetoothMessage);

	}
	else if (pianoEvent->GetInput().first == InputKey::LowerOctave) {

		piano->MoveOctave(PianoPitchMovement::Lower);
		LOG(LogLevel::Debug) << "PianoController::implementControlInstrument : lower octave.";

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::HardwareGameEvent);

		json context;
		context["Events"].push_back(string("ShiftOctave,-1"));

		meteoContextBluetoothMessage->SetContextInJson(context);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		if (outputManager != nullptr)
			outputManager->PushMessage(meteoContextBluetoothMessage);

	}
	else if (pianoEvent->GetInput().first == InputKey::RaiseOctave) {

		piano->MoveOctave(PianoPitchMovement::Raise);
		LOG(LogLevel::Debug) << "PianoController::implementControlInstrument : raise octave.";

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::HardwareGameEvent);

		json context;
		context["Events"].push_back(string("ShiftOctave,1"));

		meteoContextBluetoothMessage->SetContextInJson(context);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		if (outputManager != nullptr)
			outputManager->PushMessage(meteoContextBluetoothMessage);
	}

	return 0;
}

int PianoController::implementFastForwardControlInstrument(EventProcessor<Event>* e)
{
	if (!piano)
		return -1;

	if (!dynamic_cast<PianoEvent*>(e->GetEvent()))
		return -1;

	PianoEvent* pianoEvent = dynamic_cast<PianoEvent*>(e->GetEvent());

	if (pianoEvent->GetInput().first == InputKey::LowerOctave || pianoEvent->GetInput().first == InputKey::RaiseOctave) {
		return implementControlInstrument(e);
	}
	return 0;
}

int PianoController::implementUndoControlInstrument(EventProcessor<Event>* e)
{
	LOG(LogLevel::Depricated) << "PianoController::implementUndoControlInstrument : make pedal [" << dynamic_cast<PianoEvent*>(e->GetEvent())->GetInput().second << "] on piano [" << piano << "].";

	if (!piano)
		return -1;

	if (!dynamic_cast<PianoEvent*>(e->GetEvent()))
		return -1;

	PianoEvent* pianoEvent = dynamic_cast<PianoEvent*>(e->GetEvent());

	// TODO: 以後要寫輸入琴鍵

	if (pianoEvent->GetInput().first == InputKey::SustainPedal) {
		// 不用做事情
	}
	else if (pianoEvent->GetInput().first == InputKey::LowerOctave) {

		piano->MoveOctave(PianoPitchMovement::Raise);
		LOG(LogLevel::Debug) << "PianoController::implementUndoControlInstrument : raise octave.";

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::HardwareGameEvent);

		json context;
		context["Events"].push_back(string("ShiftOctave,1"));

		meteoContextBluetoothMessage->SetContextInJson(context);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		if (outputManager != nullptr)
			outputManager->PushMessage(meteoContextBluetoothMessage);

	}
	else if (pianoEvent->GetInput().first == InputKey::RaiseOctave) {

		piano->MoveOctave(PianoPitchMovement::Lower);
		LOG(LogLevel::Debug) << "PianoController::implementUndoControlInstrument : lower octave.";

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::HardwareGameEvent);

		json context;
		context["Events"].push_back(string("ShiftOctave,-1"));

		meteoContextBluetoothMessage->SetContextInJson(context);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		if (outputManager != nullptr)
			outputManager->PushMessage(meteoContextBluetoothMessage);
	}

	return 0;
}
