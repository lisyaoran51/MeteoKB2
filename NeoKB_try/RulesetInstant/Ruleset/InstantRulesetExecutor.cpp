#include "InstantRulesetExecutor.h"

#include "../Sheetmusic/InstantSheetmusicConverter.h"
#include "../Sheetmusic/InstantSheetmusicPostProcessor.h"
#include "../Scenes/Play/InstantPlayfield.h"
#include "../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "../Scheduler/Event/Effect/InstantFallEffect.h"
#include "../Scheduler/Event/Effect/InstantFallEffectMapper.h"
#include "../Scheduler/Event/Effect/InstantGlowLineEffectMapper.h"
#include "../Scheduler/Event/InstrumentEvents/InstantPianoSoundEventProcessor.h"
#include "../../Games/Scheduler/Event/SystemEvents/SystemEventHandler.h"
#include "../../Games/Scheduler/Event/SystemEvents/StopSystemEvent.h"
#include "../Input/InstantInputManager.h"
#include "../Timing/InstantTimeController.h"
#include "../../Framework/Timing/SpeedAdjusters/LinearSpeedAdjuster.h"
#include "Scoring/InstantScoreProcessor.h"
#include "../Scenes/Results/InstantResult.h"
#include "Replays/InstantReplayRecorder.h"




using namespace Instant::Rulesets;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::SystemEvents;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Games::Sheetmusics;
using namespace Instant::Sheetmusics;
using namespace Instant::Scenes::Play;
using namespace Games::Schedulers::Events::Effects;
using namespace Instant::Schedulers::Events::Effects;
using namespace Instant::Schedulers::Events::InstrumentEvents;
using namespace Instant::Input;
using namespace Instant::Timing;
using namespace Framework::Timing::SpeedAdjusters;
using namespace Instant::Rulesets::Scoring;
using namespace Instant::Scenes::Results;
using namespace Instant::Rulesets::Replays;






SmConverter * InstantRulesetExecutor::createSmConverter(PatternGenerator * pg)
{
	return new InstantSmConverter(pg);
}

SmPostprocessor * InstantRulesetExecutor::createSmPostprocessor()
{
	return new InstantSmPostprocessor();
}

int InstantRulesetExecutor::load()
{

	EventProcessorFilter* eProcessorFilter = GetDependencies()->GetCache<EventProcessorFilter>("EventProcessorFilter");

	// TODO: 根據有沒有拿到working sm裡面audio file決定要不要filter sound event

	if (workingSm->GetTrack() == nullptr) {
		//eProcessorFilter->AddFilterCallback(...);
	}

	return 0;
}

InstantRulesetExecutor::InstantRulesetExecutor(): RegisterType("InstantRulesetExecutor"), RulesetExecutor()
{
	// 如果要自定效果，要直接從config那裡改map algo，這邊不能動。
	eventProcessorTable["InstantFallEffect"		] = "InstantFallEffectMapper";
	eventProcessorTable["InstantGlowLineEffect"	] = "InstantGlowLineEffectMapper";
	eventProcessorTable["StopSystemEvent"		] = "SystemEventHandler";
	eventProcessorTable["InstantPianoSoundEvent"] = "InstantPianoSoundEventProcessor";

	// 註冊private load (c++才需要)
	registerLoad(bind(static_cast<int(InstantRulesetExecutor::*)(void)>(&InstantRulesetExecutor::load), this));
	constructed = false;
}

int InstantRulesetExecutor::LazyConstruct(WorkingSm * w, Ruleset* r)
{
	RulesetExecutor::LazyConstruct(w, r);
	constructed = true;
	return 0;
}

PassThroughInputManager * InstantRulesetExecutor::CreateInputManager()
{
	return new InstantInputManager(ruleset->GetRulesetInfo());
}

TimeController * InstantRulesetExecutor::CreateTimeController()
{
	return new InstantTimeController();
}

SpeedAdjuster * InstantRulesetExecutor::CreateSpeedAdjuster()
{
	return new LinearSpeedAdjuster();
}

ScoreProcessor * InstantRulesetExecutor::CreateScoreProcessor()
{
	return new InstantScoreProcessor(this);
}

ReplayRecorder * InstantRulesetExecutor::CreateReplayRecorder()
{
	//throw runtime_error("int MeteorRulesetExecutor::CreateReplayRecorder() : not implemented.");
	return new InstantReplayRecorder();
}

Result * InstantRulesetExecutor::CreateResult(Score* s)
{
	return new InstantResult(s);
}

int InstantRulesetExecutor::Elapse(MTO_FLOAT elapsedTime)
{
	// 之後可用來讀取輸入??先留著
	return 0;
}

Playfield* InstantRulesetExecutor::createPlayfield()
{
	/* 不用搞那麼複雜，直接new一個就好
	InstanceCreator<MtoObject> iCreator = InstanceCreator<MtoObject>::GetInstance();
	MeteorPlayfield* meteorPlayfield = iCreator.CreateInstance<MeteorPlayfield>("MeteorPlayfield");

	meteorPlayfield->LazyConstruct();
	*/
	return new InstantPlayfield()->SetLeaveGameFunction(leaveGame)->SetRestartGameFunction(restartGame);
}

EventProcessor<Event>* InstantRulesetExecutor::getEventProcessor(Event * e)
{
	// 為什麼不用event自己來create? 因為要去搭配不同的mapper，所以要動態調配
	string processorType = GetProcessorType(e->GetTypeName()); // .c_str();

	LOG(LogLevel::Finer) << "InstantRulesetExecutor::getEventProcessor(Event*) : event [" << e->GetStartTime() << "] has processor type [" << processorType << "] from ["<< e->GetTypeName() << "].";


	//InstanceCreator<MtoObject>& iCreator = InstanceCreator<MtoObject>::GetInstance();
	//EventProcessor<Event>* eventProcessor = iCreator.CreateInstance<EventProcessor<Event>>(processorType);
	if (processorType == "InstantFallEffectMapper") {
		int width = playfield->GetWidth();
		int height = playfield->GetHeight();
		return (new InstantFallEffectMapper(width, height))->RegisterEvent(e);
	}
	else if (processorType == "InstantGlowLineEffectMapper") {
		int width = playfield->GetWidth();
		int height = playfield->GetHeight();
		return (new InstantGlowLineEffectMapper(width, height))->RegisterEvent(e);
	}
	else if (processorType == "InstantPianoSoundEventProcessor") {
		LOG(LogLevel::Depricated) << "MeteorRulesetExecutor::getEventProcessor : getting event PianoEventProcessor at [" << e->GetStartTime() << "]";
		return (new InstantPianoSoundEventProcessor())->RegisterEvent(e);
	}
	else if (processorType == "SystemEventHandler") {
		// TODO: 在這邊把歌曲名稱擺進去
		return (new SystemEventHandler<SystemEvent>())->RegisterEvent(e);
	}


	LOG(LogLevel::Error) << "InstantRulesetExecutor::getEventProcessor() : [" << processorType << "] not found match.";
	throw runtime_error("InstantRulesetExecutor::getEventProcessor(Event*) : No matched processor type.");
	// TODO:吐錯誤訊息
	return nullptr;
}

string InstantRulesetExecutor::GetProcessorType(string eventType)
{
	map<string, string>::iterator iter = eventProcessorTable.find(eventType);
	if (iter != eventProcessorTable.end())
	{
		return eventProcessorTable[eventType];
	}


	// TODO: 吐一些錯誤訊息
	LOG(LogLevel::Error) << "InstantRulesetExecutor::GetProcessorType() : [" << eventType << "] no matched event processor.";
	return string("");
}

int InstantRulesetExecutor::playfieldLoad()
{
	RulesetExecutor<Event>::playfieldLoad();

	PeriodMap<EventProcessor<Event>*>* processors = playfield->GetEventProcessorMaster()->GetEventProcessorPeriods();

	LOG(LogLevel::Fine) << "InstantRulesetExecutor::playfieldLoad() : " << [](PeriodMap<EventProcessor<Event>*>* ps) {

		multimap<pair<float, float>, EventProcessor<Event>*>* periods = ps->GetPeriods();
		typename multimap<pair<float, float>, EventProcessor<Event>*>::iterator it;

		for (it = periods->begin(); it != periods->end(); it++) {
			LOG(LogLevel::Depricated) << "---timespan [" << it->first.first << "," << it->first.second << "] -> processor [" << it->second->GetStartTime() << "].";
		}


		return 0;
	}(processors);

	return 0;
}

