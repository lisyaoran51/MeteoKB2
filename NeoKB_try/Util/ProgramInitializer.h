#ifndef PROGRAM_INITIALIZER_H
#define PROGRAM_INITIALIZER_H



// ===============================================

// Base
#include "../Base/Game.h"
using namespace Base;

// Base/Config
#include "../Base/Config/FrameworkConfigManager.h"
using namespace Base::Config;

// Base/Play
#include "../Base/Play/Player.h"
#include "../Base/Play/Playfield.h"
#include "../Base/Play/Session.h"
using namespace Base::Play;

// Base/Scheduler
#include "../Base/Scheduler/Scheduler.h"
using namespace Base::Schedulers;

// Base/Scheduler/Events
#include "../Base/Scheduler/Event/EventProcessorMaster.h"
using namespace Base::Schedulers::Events;

// Base/Sheetmusics/Format
#include "../Base/Sheetmusic/Format/SimpleSmDecoder.h"
using namespace Base::Sheetmusics::Format;

// ===============================================

// Devices/Graphic/Renderers
#include "../Devices/Graphic/Renderers/WS2812v10Renderer.h"
using namespace Devices::Graphics::Renderers;

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