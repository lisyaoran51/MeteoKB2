#ifndef PROGRAM_INITIALIZER_H
#define PROGRAM_INITIALIZER_H



// ===============================================

// Games
//#include "../Games/Game.h"
//using namespace Games;

// Games/Config
//#include "../Games/Config/FrameworkConfigManager.h"
//using namespace Games::Config;

// Games/Play
//#include "../Games/Play/Player.h"
//#include "../Games/Play/Playfield.h"
//#include "../Games/Play/Session.h"
//using namespace Games::Play;

// Games/Scheduler
//#include "../Games/Scheduler/Scheduler.h"
//using namespace Games::Schedulers;

// Games/Scheduler/Events
#include "../Games/Scheduler/Event/EventProcessorMaster.h"
using namespace Games::Schedulers::Events;

// Games/Sheetmusics/Format
//#include "../Games/Sheetmusic/Format/SimpleSmDecoder.h"
//using namespace Games::Sheetmusics::Format;

// ===============================================

// Devices/Graphic/Renderers
//#include "../Devices/Graphic/Renderers/WS2812v10Renderer.h"
//using namespace Devices::Graphics::Renderers;

// ===============================================

// Meteor
#include "../RulesetMeteor/MeteorGame.h"
using namespace Meteor;

// Meteor/Config
#include "../RulesetMeteor/Config/MeteorConfigManager.h"
using namespace Meteor::Config;

// Meteor/Play
#include "../RulesetMeteor/Play/MeteorPlayfield.h"
using namespace Meteor::Play;

// Meteor/Rulesets
#include "../RulesetMeteor/Ruleset/MeteorRuleset.h"
#include "../RulesetMeteor/Ruleset/MeteorRulesetExecutor.h"
using namespace Meteor::Rulesets;

// Meteor/Schedulers/Events/Effects/Algorithms
#include "../RulesetMeteor/Scheduler/Event/Effect/Algorithm/ExplodeMapAlgorithm.h"
#include "../RulesetMeteor/Scheduler/Event/Effect/Algorithm/FallMapAlgorithm.h"
#include "../RulesetMeteor/Scheduler/Event/Effect/Algorithm/GlowLineMapAlgorithm.h"
#include "../RulesetMeteor/Scheduler/Event/Effect/Algorithm/TargetLineMapAlgorithm.h"
using namespace Meteor::Schedulers::Events::Effects::Algorithms;

// Meteor/Sheetmusics/Patterns
#include "../RulesetMeteor/Sheetmusic/Pattern/MeteorPatternGenerator.h"
using namespace Meteor::Sheetmusics::Patterns;






namespace Util {

	class ProgramInitializer {

	public:

		static int Initialize();

	};

}





#endif