#include "RecordDynamicEventGenerator.h"


#include "../../../Games/Scheduler/Event/SystemEvents/StopSystemEvent.h"
#include "../../../Games/Scheduler/Event/SystemEvents/EndSystemEvent.h"
#include "../../../Games/Scheduler/Event/SystemEvents/RestartSystemEvent.h"
#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Util/StringSplitter.h"
#include <string>


using namespace Record::Schedulers::Events;
using namespace Games::Schedulers::Events::SystemEvents;
using namespace Games::Output::Bluetooths;
using namespace Util;
using namespace std;



RecordDynamicEventGenerator::RecordDynamicEventGenerator(Playfield * p) : RegisterType("RecordDynamicEventGenerator"), DynamicEventGenerator(p)
{
}

int RecordDynamicEventGenerator::OnMessage(MeteoBluetoothMessage * message)
{
	if (message->GetCommand() == MeteoCommand::InstantEvent) {

		MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);
		json context = contextMessage->GetContextInJson();

		// {Command:["Fall,48,2,1","Sound,48,2,105","Line,48,1,0.5","Stop,1,3"...]}
		// 

		for (int i = 0; i < context["Command"].size(); i++) {
			string command = context["Command"][i].get<string>();

			vector<string> splitCommand = StringSplitter::Split(command, ",");

			if (splitCommand[0] == "Stop") {

				double startTime = GetClock()->GetCurrentTime() + stod(splitCommand[1]);

				double lifeTime = stod(splitCommand[2]);

				StopSystemEvent* stopSystemEvent = new StopSystemEvent(startTime, lifeTime);

				unique_lock<mutex> uLock(dynamicEventsMutex);

				dynamicEvents.push_back(stopSystemEvent);
			}
			else if (splitCommand[0] == "End") {

				double startTime = GetClock()->GetCurrentTime() + stod(splitCommand[1]);

				double lifeTime = stod(splitCommand[2]);

				EndSystemEvent* restartSystemEvent = new EndSystemEvent(startTime, lifeTime);

				unique_lock<mutex> uLock(dynamicEventsMutex);

				dynamicEvents.push_back(restartSystemEvent);
			}
			else if (splitCommand[0] == "Restart") {

				double startTime = GetClock()->GetCurrentTime() + stod(splitCommand[1]);

				double lifeTime = stod(splitCommand[2]);

				RestartSystemEvent* restartSystemEvent = new RestartSystemEvent(startTime, lifeTime);

				unique_lock<mutex> uLock(dynamicEventsMutex);

				dynamicEvents.push_back(restartSystemEvent);
			}
		}
	}


	return 0;
}