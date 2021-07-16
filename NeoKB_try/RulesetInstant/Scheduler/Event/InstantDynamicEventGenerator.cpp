#include "InstantDynamicEventGenerator.h"

#include "Effect/InstantFallEffect.h"
#include "Effect/InstantSpotEffect.h"
#include "InstrumentEvents/InstantPianoSoundEvent.h"
#include "InstrumentEvents/InstantPianoEvent.h"
#include "PlayfieldEvents/InstantOctaveShiftEvent.h"
#include "../../../Games/Scheduler/Event/SystemEvents/StopSystemEvent.h"
#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Util/StringSplitter.h"
#include <string>
#include "../../Scenes/Play/InstantPlayfield.h"


using namespace Instant::Schedulers::Events;
using namespace Instant::Schedulers::Events::Effects;
using namespace Instant::Schedulers::Events::InstrumentEvents;
using namespace Instant::Schedulers::Events::PlayfieldEvents;
using namespace Games::Schedulers::Events::SystemEvents;
using namespace Games::Output::Bluetooths;
using namespace Util;
using namespace std;
using namespace Instant::Scenes::Play;



InstantDynamicEventGenerator::InstantDynamicEventGenerator(Playfield * p) : RegisterType("InstantDynamicEventGenerator"), DynamicEventGenerator(p)
{
	isInputable = true;
	isPresent = true;
	fallSpeed = dynamic_cast<InstantPlayfield*>(p)->GetWorkingSm()->GetSm()->GetSmInfo()->difficuty->Speed;
}

int InstantDynamicEventGenerator::SetPlayfield(Playfield * p)
{
	playfield = p;
	return 0;
}

int InstantDynamicEventGenerator::onMessage(MeteoBluetoothMessage * message)
{

	LOG(LogLevel::Debug) << "InstantDynamicEventGenerator::onMessage() : get bt message.";
	if (message->GetCommand() == MeteoCommand::AppGameEvent) {

		MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);
		json context = contextMessage->GetContextInJson();

		// {Events:["Fall,48,1"]}
		// 
		if (context.contains("Events") == 0) {
			LOG(LogLevel::Warning) << "InstantDynamicEventGenerator::onMessage() : format error.";
			return -1;
		}

		if (context["Events"].size() == 0) {
			LOG(LogLevel::Warning) << "InstantDynamicEventGenerator::onMessage() : not event inside.";
			return -1;
		}

		string gameEventContext = context["Events"][0];
		vector<string> parameters = StringSplitter::Split(gameEventContext, ",");

		if (parameters.size() == 0)
			return -1;

		if (parameters[0] == "FallEffect") {

			if (parameters.size() < 3)
				return -1;

			int key = stoi(parameters[1]);

			if (key < 12 || key > 83)
				return -1;

			float time = stof(parameters[2]);
			if (time <= 0)
				time = 16.0 / fallSpeed;

			switch (key % 12) {
			case 1:
			case 3:
			case 6:
			case 8:
			case 10:
				time += 16.0 / fallSpeed * 5.0 / 16.0;
				break;
			}

			InstantFallEffect* instantFallEffect = new InstantFallEffect(Pitch(key), GetClock()->GetCurrentTime() + time, 16.0 / fallSpeed + time, fallSpeed);

			unique_lock<mutex> uLock(dynamicEventsMutex);

			dynamicEvents.push_back(instantFallEffect);

		}

		if (parameters[0] == "SpotEffect") {

			if (parameters.size() < 3)
				return -1;

			int key = stoi(parameters[1]);

			if (key < 12 || key > 83)
				return -1;

			float time = stof(parameters[2]);
			if (time <= 0)
				time = 1;

			InstantSpotEffect* instantSpotEffect = new InstantSpotEffect(Pitch(key), GetClock()->GetCurrentTime(), time);

			unique_lock<mutex> uLock(dynamicEventsMutex);

			dynamicEvents.push_back(instantSpotEffect);

		}

		if (parameters[0] == "AppPlaySound") {

			if (parameters.size() < 3)
				return -1;

			int key = stoi(parameters[1]);

			if (key < 12 || key > 83)
				return -1;

			float volume = stof(parameters[2]);

			if (volume < 0 || volume > 1)
				volume = 0.3;

			InstantPianoSoundEvent* instantPianoSoundEventKeyDown = new InstantPianoSoundEvent(pair<Pitch, float>((Pitch)key, volume), GetClock()->GetCurrentTime(), 0);

			unique_lock<mutex> uLock(dynamicEventsMutex);

			dynamicEvents.push_back(instantPianoSoundEventKeyDown);
		}

		if (parameters[0] == "AppStopSound") {

			if (parameters.size() < 2)
				return -1;

			int key = stoi(parameters[1]);

			if (key < 12 || key > 83)
				return -1;

			InstantPianoSoundEvent* instantPianoSoundEventKeyUp = new InstantPianoSoundEvent(pair<Pitch, float>((Pitch)key, 0), GetClock()->GetCurrentTime(), 0);

			unique_lock<mutex> uLock(dynamicEventsMutex);

			dynamicEvents.push_back(instantPianoSoundEventKeyUp);
		}

		if (parameters[0] == "AppPedalDown") {

			InstantPianoSoundEvent* instantPianoSoundEventPedalDown = new InstantPianoSoundEvent(true, GetClock()->GetCurrentTime(), 0);
			InstantPianoEvent* instantPianoEventPedalDown = new InstantPianoEvent(pair<InputKey, int>(InputKey::SustainPedal, 1), GetClock()->GetCurrentTime(), 0);

			unique_lock<mutex> uLock(dynamicEventsMutex);

			dynamicEvents.push_back(instantPianoSoundEventPedalDown);
			dynamicEvents.push_back(instantPianoEventPedalDown);
		}

		if (parameters[0] == "AppPedalUp") {

			InstantPianoSoundEvent* instantPianoSoundEventPedalUp = new InstantPianoSoundEvent(false, GetClock()->GetCurrentTime(), 0);
			InstantPianoEvent* instantPianoEventPedalUp = new InstantPianoEvent(pair<InputKey, int>(InputKey::SustainPedal, -1), GetClock()->GetCurrentTime(), 0);

			unique_lock<mutex> uLock(dynamicEventsMutex);

			dynamicEvents.push_back(instantPianoSoundEventPedalUp);
			dynamicEvents.push_back(instantPianoEventPedalUp);
		}

		if (parameters[0] == "ShiftOctave") {

			if (parameters.size() < 2)
				return -1;

			int shiftDirection = stoi(parameters[1]);

			if (shiftDirection != 1 && shiftDirection != -1)
				return -1;

			InstantOctaveShiftEvent* instantOctaveShift = nullptr;
			InstantPianoEvent* instantPianoEventOctaveShift = nullptr;

			if (shiftDirection == 1) {
				instantOctaveShift = new InstantOctaveShiftEvent(InstantOctaveShiftType::Raise, GetClock()->GetCurrentTime(), 0);
				instantPianoEventOctaveShift = new InstantPianoEvent(pair<InputKey, int>(InputKey::RaiseOctave, 1), GetClock()->GetCurrentTime(), 0);
			}
			else if (shiftDirection == -1) {
				instantOctaveShift = new InstantOctaveShiftEvent(InstantOctaveShiftType::Lower, GetClock()->GetCurrentTime(), 0);
				instantPianoEventOctaveShift = new InstantPianoEvent(pair<InputKey, int>(InputKey::LowerOctave, 1), GetClock()->GetCurrentTime(), 0);
			}

			unique_lock<mutex> uLock(dynamicEventsMutex);

			dynamicEvents.push_back(instantOctaveShift);
			dynamicEvents.push_back(instantPianoEventOctaveShift);
		}

		// TODO: Stop Spot Effect
	}


	return 0;
}
