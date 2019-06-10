#include "ProgramInitializer.h"

#include "Log.h"
#include "InstanceCreator.h"
#include "MtoObject.h"

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
using namespace Meteor::Schedulers::Events::Effects::Algorithms;

// Meteor/Sheetmusics/Patterns
#include "../RulesetMeteor/Sheetmusic/Pattern/MeteorPatternGenerator.h"
using namespace Meteor::Sheetmusics::Patterns;




using namespace Util;

LogLevel Util::logLevel = LogLevel::Info;

int ProgramInitializer::Initialize()
{
	// TODO: 讀取文件決定要用什麼level
	Util::logLevel = LogLevel::Info;

	LOG(LogLevel::Fine) << "int ProgramInitializer::Initialize() : Start initializing program...";

	InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();

	// 1.建instance creator
	// TODO: 應該要寫個程式來自動掃描所有code裡面有哪些mto object要加入instance creator

	// ===============================================

	// Base
	iCreator.RegisterType<Game>("Game");

	// Base/Config
	iCreator.RegisterType<FrameworkConfigManager>("FrameworkConfigManager");

	// Base/Play
	iCreator.RegisterType<Player>("Player");

	// Base/Scheduler
	iCreator.RegisterType<Scheduler>("Scheduler");

	// Base/Scheduler/Events
	iCreator.RegisterType<EventProcessorMaster>("EventProcessorMaster");

	// Base/Sheetmusics/Format
	iCreator.RegisterType<SimpleSmDecoder>("SimpleSmDecoder");

	// ===============================================

	// Devices/Graphic/Renderers
	iCreator.RegisterType<WS2812v10Renderer>("WS2812v10Renderer");

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

	SmDecoder::RegisterDecoder("simple file format v0", "SimpleSmDecoder");

	return 0;
}
