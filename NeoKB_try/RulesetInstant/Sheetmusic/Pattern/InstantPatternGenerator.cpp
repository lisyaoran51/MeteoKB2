#include "InstantPatternGenerator.h"

#include "../../../Util/MtoType.h"
#include "../../../Games/Scheduler/Event/ControlPoints/NoteControlPoint.h"
#include "../../../Games/Scheduler/Event/ControlPoints/InputKeyControlPoint.h"
#include "../../Scheduler/Event/Effect/FallEffect.h"
#include "../../../Games/Scheduler/Event/GameEvents/StartGameEvent.h"
#include "../../../Games/Scheduler/Event/SystemEvents/SystemEvent.h"
#include "../../../Games/Scheduler/Event/SystemEvents/StopSystemEvent.h"
#include "../../../Instruments/Pitch.h"



using namespace Instant::Sheetmusics::Patterns;
using namespace Instant::Config;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Instant::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events::GameEvents;
using namespace Games::Schedulers::Events::SystemEvents;
using namespace Instruments;



int InstantPatternGenerator::load()
{

	InstantConfigManager * m = GetCache<MeteorConfigManager>("InstantConfigManager");

	if (!m)
		throw runtime_error("int InstantPatternGenerator::load() : InstantConfigManager not found in cache.");

	return load(m);
}

int InstantPatternGenerator::load(InstantConfigManager * mcf)
{

	return 0;
}

InstantPatternGenerator::InstantPatternGenerator(): PatternGenerator(), RegisterType("InstantPatternGenerator")
{
	registerLoad(bind(static_cast<int(InstantPatternGenerator::*)(void)>(&InstantPatternGenerator::load), this));
}

int InstantPatternGenerator::Initialize(Sm<Event>* s)
{
	PatternGenerator::Initialize(s);

	return 0;
}

Pattern* InstantPatternGenerator::Generate(vector<Event*>* es, Event * e)
{
	
	return nullptr;

}

int InstantPatternGenerator::CreateOtherEvent(vector<Event*>* es)
{
	
	return 0;
}

int InstantPatternGenerator::PostProcess()
{
	for (int i = 0; i < patterns.size(); i++) {



	}



	return 0;
}

