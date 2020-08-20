#include"InstantSheetmusicConverter.h"

#include "../../Games/Scheduler/Event/Effect/Effect.h"
#include "../../Games/Scheduler/Event/ControlPoints/NoteControlPoint.h"
#include "../../Games/Scheduler/Event/ControlPoints/InputKeyControlPoint.h"
#include "../../Games/Scheduler/Event/ControlPoints/OctaveAutoControlPoint.h"
#include "../../Games/Scheduler/Event/GameEvents/StartGameEvent.h"




using namespace Instant::Sheetmusics;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Games::Schedulers::Events::GameEvents;



InstantSmConverter::InstantSmConverter(PatternGenerator * pg): SmConverter(pg)
{
}

Sm<Event>* InstantSmConverter::Convert(Sm<Event>* s)
{
	LOG(LogLevel::Fine) << "MeteorSmConverter::Convert(Sm<Event>*) : Start converting sheetmusic...";
	Sm<Event>* convertedSm = SmConverter::Convert(s);


	return convertedSm;
}

int InstantSmConverter::convertEvent(vector<Event*>* es, Event* e)
{


	return 0;
}
