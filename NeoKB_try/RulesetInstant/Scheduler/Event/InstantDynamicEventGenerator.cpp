#include "InstantDynamicEventGenerator.h"

#include "Effect/InstantFallEffect.h"
#include "Effect/InstantGlowLineEffect.h"
#include "InstrumentEvents/InstantPianoSoundEvent.h"
#include "../../../Games/Scheduler/Event/SystemEvents/StopSystemEvent.h"
#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Util/StringSplitter.h"
#include <string>


using namespace Instant::Schedulers::Events;
using namespace Instant::Schedulers::Events::Effects;
using namespace Instant::Schedulers::Events::InstrumentEvents;
using namespace Games::Schedulers::Events::SystemEvents;
using namespace Games::Output::Bluetooths;
using namespace Util;
using namespace std;



InstantDynamicEventGenerator::InstantDynamicEventGenerator(Playfield * p) : RegisterType("InstantDynamicEventGenerator"), DynamicEventGenerator(p)
{
}

int InstantDynamicEventGenerator::OnMessage(MeteoBluetoothMessage * message)
{
	if (message->GetCommand() == MeteoCommand::InstantEvent) {

		MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);
		json context = contextMessage->GetContextInJson();

		// {Command:["Fall,48,2,1","Sound,48,2,105","Line,48,1,0.5","Stop,1,3"...]}
		// 

		for (int i = 0; i < context["Command"].size(); i++) {
			string command = context["Command"][i].get<string>();

			vector<string> splitCommand = StringSplitter::Split(command, ",");

			if (splitCommand[0] == "Fall") {

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



	return 0;
}
