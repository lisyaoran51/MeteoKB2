#include "InstantDynamicEventGenerator.h"

#include "Effect/InstantFallEffect.h"
#include "Effect/InstantSpotEffect.h"
#include "InstrumentEvents/InstantPianoSoundEvent.h"
#include "../../../Games/Scheduler/Event/SystemEvents/StopSystemEvent.h"
#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Util/StringSplitter.h"
#include <string>
#include "../../Scenes/Play/InstantPlayfield.h"


using namespace Instant::Schedulers::Events;
using namespace Instant::Schedulers::Events::Effects;
using namespace Instant::Schedulers::Events::InstrumentEvents;
using namespace Games::Schedulers::Events::SystemEvents;
using namespace Games::Output::Bluetooths;
using namespace Util;
using namespace std;
using namespace Instant::Scenes::Play;



InstantDynamicEventGenerator::InstantDynamicEventGenerator(Playfield * p) : RegisterType("InstantDynamicEventGenerator"), DynamicEventGenerator(p)
{
	fallSpeed = dynamic_cast<InstantPlayfield*>(p)->GetWorkingSm()->GetSm()->GetSmInfo()->difficuty->Speed;
}

int InstantDynamicEventGenerator::SetPlayfield(Playfield * p)
{
	playfield = p;
	return 0;
}

int InstantDynamicEventGenerator::onMessage(MeteoBluetoothMessage * message)
{
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

			InstantFallEffect* instantFallEffect = new InstantFallEffect(Pitch(key), GetClock()->GetCurrentTime() + time, 16.0 / fallSpeed, fallSpeed);

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

			unique_lock<mutex> uLock(dynamicEventsMutex);

			dynamicEvents.push_back(instantPianoSoundEventPedalDown);
		}

		if (parameters[0] == "AppPedalUp") {

			InstantPianoSoundEvent* instantPianoSoundEventPedalUp = new InstantPianoSoundEvent(false, GetClock()->GetCurrentTime(), 0);

			unique_lock<mutex> uLock(dynamicEventsMutex);

			dynamicEvents.push_back(instantPianoSoundEventPedalUp);
		}

		// TODO: Stop Spot Effect


		for (int i = 0; i < context["Events"].size(); i++) {
			string command = context["Events"][i].get<string>();

			vector<string> splitCommand = StringSplitter::Split(command, ",");

			if (splitCommand[0] == "FallEffect") {

				int key = stoi(splitCommand[1]);

				double startTime = GetClock()->GetCurrentTime() + stod(splitCommand[2]);

				double lifeTime = stod(splitCommand[3]);

				int speed = 1.0 / lifeTime * 16;

				// 如果是黑鍵，就要稍微往後移一點，不然會提早掉下來
				switch (key % 12) {
				case 1:
				case 3:
				case 6:
				case 8:
				case 10:
					startTime += lifeTime * 5.0 / 16.0;
					break;
				}

				InstantFallEffect* instantFallEffect = new InstantFallEffect(key, 0, startTime, lifeTime, speed);

				unique_lock<mutex> uLock(dynamicEventsMutex);

				dynamicEvents.push_back(instantFallEffect);
			}
			else if (splitCommand[0] == "Line") {
				int key = stoi(splitCommand[1]);

				double startTime = GetClock()->GetCurrentTime() + stod(splitCommand[2]);

				double lifeTime = stod(splitCommand[3]);

				InstantGlowLineEffect* instantGlowLineEffect = new InstantGlowLineEffect(key, 0, startTime, lifeTime);

				unique_lock<mutex> uLock(dynamicEventsMutex);

				dynamicEvents.push_back(instantGlowLineEffect);
			}
			else if (splitCommand[0] == "Sound") {
				int key = stoi(splitCommand[1]);

				double startTime = GetClock()->GetCurrentTime() + stod(splitCommand[2]);

				int volume = stoi(splitCommand[3]);

				if (key == -1) {
					InstantPianoSoundEvent* instantPianoSoundEvent = new InstantPianoSoundEvent(volume == 0, startTime, 1);

					unique_lock<mutex> uLock(dynamicEventsMutex);

					dynamicEvents.push_back(instantPianoSoundEvent);
				}
				else {
					InstantPianoSoundEvent* instantPianoSoundEventDown = new InstantPianoSoundEvent(pair<Pitch, float>((Pitch)key, float(volume) / 128.f), startTime, 1);

					InstantPianoSoundEvent* instantPianoSoundEventUp = new InstantPianoSoundEvent(pair<Pitch, float>((Pitch)key, 0), startTime + 0.5, 1);

					unique_lock<mutex> uLock(dynamicEventsMutex);

					dynamicEvents.push_back(instantPianoSoundEventDown);
					dynamicEvents.push_back(instantPianoSoundEventUp);
				}
			}
			else if (splitCommand[0] == "Stop") {

				double startTime = GetClock()->GetCurrentTime() + stod(splitCommand[1]);

				double lifeTime = stod(splitCommand[2]);

				StopSystemEvent* stopSystemEvent = new StopSystemEvent(startTime, lifeTime);

				unique_lock<mutex> uLock(dynamicEventsMutex);

				dynamicEvents.push_back(stopSystemEvent);
			}
		}
	}


	return 0;
}
