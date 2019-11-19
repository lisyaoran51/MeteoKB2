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

// Games/Sheetmusics/Format
#include "../Games/Sheetmusic/Format/SimpleSmDecoder.h"
using namespace Games::Sheetmusics::Format;

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
using namespace Meteor::Schedulers::Events::Effects::Algorithms;

// Meteor/Sheetmusics/Patterns
#include "../RulesetMeteor/Sheetmusic/Pattern/MeteorPatternGenerator.h"
using namespace Meteor::Sheetmusics::Patterns;




using namespace Util;



int ProgramInitializer::Initialize()
{
	// TODO: 讀取文件決定要用什麼level
	Util::logLevel = LogLevel::Info;

	LOG(LogLevel::Fine) << "int ProgramInitializer::Initialize() : Start initializing program...";

	InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();

	// 1.建instance creator
	// TODO: 應該要寫個程式來自動掃描所有code裡面有哪些mto object要加入instance creator

	// ===============================================

	// Games
	iCreator.RegisterType<Game>("Game");

	// Games/Config
	iCreator.RegisterType<FrameworkConfigManager>("FrameworkConfigManager");

	// Games/Play
	//iCreator.RegisterType<Player>("Player");

	// Games/Scheduler
	iCreator.RegisterType<Scheduler>("Scheduler");

	// Games/Scheduler/Events
	iCreator.RegisterType<EventProcessorMaster>("EventProcessorMaster");

	// Games/Scheduler/Events/Effects/Algorithms
	iCreator.RegisterType<MapPitchShifter>("MapPitchShifter");
	iCreator.RegisterType<LinearMapPitchShifter>("LinearMapPitchShifter");

	// Games/Sheetmusics/Format
	iCreator.RegisterType<SimpleSmDecoder>("SimpleSmDecoder");

	// ===============================================

	// Devices/Graphic/Renderers
	//iCreator.RegisterType<WS2812v10Renderer>("WS2812v10Renderer");

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

	// Meteor/Schedulers/Events/Effects/Algorithms
	iCreator.RegisterType<ExplodeMapAlgorithm>("ExplodeMapAlgorithm");
	iCreator.RegisterType<FallMapAlgorithm>("FallMapAlgorithm");
	iCreator.RegisterType<GlowLineMapAlgorithm>("GlowLineMapAlgorithm");
	iCreator.RegisterType<TargetLineMapAlgorithm>("TargetLineMapAlgorithm");

	// Meteor/Sheetmusics/Patterns
	iCreator.RegisterType<MeteorPatternGenerator>("MeteorPatternGenerator");

	iCreator.PrintCreators();

	// ===============================================



	// 2.建decoder
	// TODO: 應該要從文件讀取要註冊哪些decoder

	//SmDecoder::RegisterDecoder("simple file format v0", "SimpleSmDecoder");

	return 0;
}
