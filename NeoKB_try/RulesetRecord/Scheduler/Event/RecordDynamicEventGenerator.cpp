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

int RecordDynamicEventGenerator::onMessage(MeteoBluetoothMessage * message)
{
	

	return 0;
}
