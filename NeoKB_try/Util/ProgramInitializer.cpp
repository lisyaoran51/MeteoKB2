#include "ProgramInitializer.h"

#include "Log.h"
#include "InstanceCreator.h"
//#include "MtoObject.h"

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

#include "../Games/Scheduler/Event/Effect/Algorithm/MapPitchShifter.h"
#include "../Games/Scheduler/Event/Effect/Algorithm/LinearMapPitchShifter.h"
using namespace Games::Schedulers::Events::Effects::Algorithms;

#include "../Games/Scheduler/Event/SystemEvents/SystemControllers/SystemController.h"
using namespace Games::Schedulers::Events::SystemEvents::SystemControllers;

// Games/Sheetmusics/Format
#include "../Games/Sheetmusic/Format/SimpleSmDecoder.h"
using namespace Games::Sheetmusics::Format;

// ===============================================

// Instruments/Audio/Format
#include "../Instruments/Audio/Format/SimpleSoundBindingSetDecoder.h"
using namespace Instruments::Audio::Format;

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

// Meteor/Rulesets/Modifiers
#include "../RulesetMeteor/Ruleset/Modifiers/ClockRateModifier.h"
#include "../RulesetMeteor/Ruleset/Modifiers/FallSpeedModifier.h"
#include "../RulesetMeteor/Ruleset/Modifiers/HandDifficultyModifier.h"
#include "../RulesetMeteor/Ruleset/Modifiers/MusicGameModifier.h"
#include "../RulesetMeteor/Ruleset/Modifiers/RepeatPracticeModifier.h"
#include "../RulesetMeteor/Ruleset/Modifiers/WhiteKeyTargetLineModifier.h"
using namespace Meteor::Rulesets::Modifiers;

// Meteor/Schedulers/Events/ControlPoints
#include "../RulesetMeteor/Scheduler/Event/ControlPoints/MeteorNoteControlPointHitObject.h"
using namespace Meteor::Schedulers::Events::ControlPoints;

// Meteor/Schedulers/Events/Effects/Algorithms
#include "../RulesetMeteor/Scheduler/Event/Effect/Algorithm/ExplodeMapAlgorithm.h"
#include "../RulesetMeteor/Scheduler/Event/Effect/Algorithm/FallMapAlgorithm.h"
#include "../RulesetMeteor/Scheduler/Event/Effect/Algorithm/GlowLineMapAlgorithm.h"
using namespace Meteor::Schedulers::Events::Effects::Algorithms;

// Meteor/Sheetmusics/Patterns
#include "../RulesetMeteor/Sheetmusic/Pattern/MeteorPatternGenerator.h"
using namespace Meteor::Sheetmusics::Patterns;

// ===============================================

// Instant
#include "../RulesetInstant/InstantGame.h"
using namespace Instant;

// Instant/Config
#include "../RulesetInstant/Config/InstantConfigManager.h"
using namespace Instant::Config;

// Instant/Play
#include "../RulesetInstant/Scenes/Play/InstantPlayfield.h"
using namespace Instant::Scenes::Play;

// Instant/Rulesets
#include "../RulesetInstant/Ruleset/InstantRuleset.h"
#include "../RulesetInstant/Ruleset/InstantRulesetExecutor.h"
using namespace Instant::Rulesets;

// Instant/Schedulers/Events/Effects/Algorithms
#include "../RulesetInstant/Scheduler/Event/Effect/Algorithm/InstantFallMapAlgorithm.h"
#include "../RulesetInstant/Scheduler/Event/Effect/Algorithm/InstantGlowLineMapAlgorithm.h"
using namespace Instant::Schedulers::Events::Effects::Algorithms;

// Instant/Schedulers/Events/InstrumentEvents/InstrumentControllers
#include "../RulesetInstant/Scheduler/Event/InstrumentEvents/InstrumentControllers/InstantVirtualPianoController.h"
using namespace Instant::Schedulers::Events::InstrumentEvents::InstrumentControllers;

using namespace Util;



int ProgramInitializer::Initialize()
{

	LOG(LogLevel::Fine) << "int ProgramInitializer::Initialize() : Start initializing program...";

	InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();

	// 1.建instance creator
	// TODO: 應該要寫個程式來自動掃描所有code裡面有哪些mto object要加入instance creator

	// ===============================================

	// Games
	//iCreator.RegisterType<Game>("Game");

	// Games/Config
	//iCreator.RegisterType<FrameworkConfigManager>("FrameworkConfigManager");

	// Games/Play
	//iCreator.RegisterType<Player>("Player");

	// Games/Scheduler
	//iCreator.RegisterType<Scheduler>("Scheduler");

	// Games/Scheduler/Events
	iCreator.RegisterType<EventProcessorMaster>("EventProcessorMaster");

	// Games/Scheduler/Events/Effects/Algorithms
	iCreator.RegisterType<LinearMapPitchShifter>("LinearMapPitchShifter");

	// Games/Scheduler/Events/SystemEvents/SystemControllers
	iCreator.RegisterType<SystemController>("SystemController");

	// Games/Sheetmusics/Format
	iCreator.RegisterType<SimpleSmDecoder>("SimpleSmDecoder");

	// ===============================================

	// Devices/Graphic/Renderers
	//iCreator.RegisterType<WS2812v10Renderer>("WS2812v10Renderer");

	// ===============================================

	// Instruments/Audio/Format
	iCreator.RegisterType<SimpleSoundBindingSetDecoder>("SimpleSoundBindingSetDecoder");

	// ===============================================

	// Meteor
	iCreator.RegisterType<MeteorGame>("MeteorGame");

	// Meteor/Config
	iCreator.RegisterType<MeteorConfigManager>("MeteorConfigManager");

	// Meteor/Play
	iCreator.RegisterType<MeteorPlayfield>("MeteorPlayfield");

	// Meteor/Rulesets
	iCreator.RegisterType<MeteorRuleset>("MeteorRuleset");
	iCreator.RegisterType<MeteorRulesetExecutor>("MeteorRulesetExecutor");

	// Meteor/Rulesets
	iCreator.RegisterType<ClockRateModifier>("ClockRateModifier");
	iCreator.RegisterType<FallSpeedModifier>("FallSpeedModifier");
	iCreator.RegisterType<HandDifficultyModifier>("HandDifficultyModifier");
	iCreator.RegisterType<MusicGameModifier>("MusicGameModifier");
	iCreator.RegisterType<RepeatPracticeModifier>("RepeatPracticeModifier");
	iCreator.RegisterType<WhiteKeyTargetLineModifier>("WhiteKeyTargetLineModifier");


	// Meteor/Schedulers/Events/Effects/Algorithms
	iCreator.RegisterType<ExplodeMapAlgorithm>("ExplodeMapAlgorithm");
	iCreator.RegisterType<FallMapAlgorithm>("FallMapAlgorithm");
	iCreator.RegisterType<EruptMapAlgorithm>("EruptMapAlgorithm");
	iCreator.RegisterType<GlowLineMapAlgorithm>("GlowLineMapAlgorithm");
	iCreator.RegisterType<TargetLineMapAlgorithm>("TargetLineMapAlgorithm");

	// Meteor/Schedulers/Events/IoEvents/IoCommunicators
	iCreator.RegisterType<SustainPedalLightRingIoCommunicator>("SustainPedalLightRingIoCommunicator");

	// Meteor/Schedulers/Events/InstrumentEvents/InstrumentControllers
	iCreator.RegisterType<PianoController>("PianoController");

	// Meteor/Schedulers/Events/PlayfieldEvents/PlayfieldControllers
	iCreator.RegisterType<OctaveShifter>("OctaveShifter");

	// Meteor/Schedulers/Events/TimeEvents/TimeControllerControllers
	iCreator.RegisterType<OctaveShifter>("RepeatPracticeController");

	// Meteor/Sheetmusics/Patterns
	iCreator.RegisterType<MeteorPatternGenerator>("MeteorPatternGenerator");


	// ===============================================


	// ===============================================

	// Instant
	iCreator.RegisterType<InstantGame>("InstantGame");

	// Instant/Config
	iCreator.RegisterType<InstantConfigManager>("InstantConfigManager");

	// Instant/Play
	iCreator.RegisterType<InstantPlayfield>("InstantPlayfield");

	// Instant/Rulesets
	iCreator.RegisterType<InstantRuleset>("InstantRuleset");
	iCreator.RegisterType<InstantRulesetExecutor>("InstantRulesetExecutor");

	// Instant/Schedulers/Events/Effects/Algorithms
	iCreator.RegisterType<InstantFallMapAlgorithm>("InstantFallMapAlgorithm");
	iCreator.RegisterType<InstantGlowLineMapAlgorithm>("InstantGlowLineMapAlgorithm");

	// Instant/Schedulers/Events/InstrumentEvents/InstrumentControllers
	iCreator.RegisterType<InstantVirtualPianoController>("InstantVirtualPianoController");



	// 2.建decoder
	// TODO: 應該要從文件讀取要註冊哪些decoder

	//SmDecoder::RegisterDecoder("simple file format v0", "SimpleSmDecoder");

	iCreator.PrintCreators();
	return 0;
}
