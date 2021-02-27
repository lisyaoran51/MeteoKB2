#include "RecordRulesetExecutor.h"

#include "../Sheetmusic/RecordSheetmusicConverter.h"
#include "../Sheetmusic/RecordSheetmusicPostProcessor.h"
#include "../Scenes/Play/RecordPlayfield.h"
#include "../../Games/Scheduler/Event/SystemEvents/StopSystemEventHandler.h"
#include "../../Games/Scheduler/Event/SystemEvents/EndSystemEventHandler.h"
#include "../../Games/Scheduler/Event/SystemEvents/RestartSystemEventHandler.h"
#include "../Input/RecordInputManager.h"
#include "../Timing/RecordTimeController.h"
#include "../../Framework/Timing/SpeedAdjusters/LinearSpeedAdjuster.h"
#include "Scoring/RecordScoreProcessor.h"
#include "../Scenes/Results/RecordResult.h"
#include "Replays/RecordReplayRecorder.h"




using namespace Record::Rulesets;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::SystemEvents;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Games::Sheetmusics;
using namespace Record::Sheetmusics;
using namespace Record::Scenes::Play;
using namespace Record::Input;
using namespace Record::Timing;
using namespace Framework::Timing::SpeedAdjusters;
using namespace Record::Rulesets::Scoring;
using namespace Record::Scenes::Results;
using namespace Record::Rulesets::Replays;






SmConverter * RecordRulesetExecutor::createSmConverter(PatternGenerator * pg)
{
	return new RecordSmConverter(pg);
}

SmPostprocessor * RecordRulesetExecutor::createSmPostprocessor()
{
	return new RecordSmPostprocessor();
}

int RecordRulesetExecutor::load()
{

	EventProcessorFilter* eProcessorFilter = GetDependencies()->GetCache<EventProcessorFilter>("EventProcessorFilter");

	// TODO: 根據有沒有拿到working sm裡面audio file決定要不要filter sound event

	if (workingSm->GetTrack() == nullptr) {
		//eProcessorFilter->AddFilterCallback(...);
	}

	return 0;
}

RecordRulesetExecutor::RecordRulesetExecutor(): RegisterType("RecordRulesetExecutor"), RulesetExecutor()
{
	// 如果要自定效果，要直接從config那裡改map algo，這邊不能動。
	eventProcessorTable["StopSystemEvent"		] = "StopSystemEventHandler";
	eventProcessorTable["EndSystemEvent"		] = "EndSystemEventHandler";
	eventProcessorTable["RestartSystemEvent"	] = "RestartSystemEventHandler";

	// 註冊private load (c++才需要)
	registerLoad(bind(static_cast<int(RecordRulesetExecutor::*)(void)>(&RecordRulesetExecutor::load), this));
	constructed = false;
}

int RecordRulesetExecutor::LazyConstruct(WorkingSm * w, Ruleset* r)
{
	RulesetExecutor::LazyConstruct(w, r);
	constructed = true;
	return 0;
}

PassThroughInputManager * RecordRulesetExecutor::CreateInputManager()
{
	return new RecordInputManager(ruleset->GetRulesetInfo());
}

TimeController * RecordRulesetExecutor::CreateTimeController()
{
	return new RecordTimeController();
}

SpeedAdjuster * RecordRulesetExecutor::CreateSpeedAdjuster()
{
	return new LinearSpeedAdjuster();
}

ScoreProcessor * RecordRulesetExecutor::CreateScoreProcessor()
{
	return new RecordScoreProcessor(this);
}

ReplayRecorder * RecordRulesetExecutor::CreateReplayRecorder()
{
	//throw runtime_error("int MeteorRulesetExecutor::CreateReplayRecorder() : not implemented.");
	return new RecordReplayRecorder();
}

Result * RecordRulesetExecutor::CreateResult(Score* s)
{
	return new RecordResult(s);
}

int RecordRulesetExecutor::Elapse(MTO_FLOAT elapsedTime)
{
	// 之後可用來讀取輸入??先留著
	return 0;
}

Playfield* RecordRulesetExecutor::createPlayfield()
{
	/* 不用搞那麼複雜，直接new一個就好
	InstanceCreator<MtoObject> iCreator = InstanceCreator<MtoObject>::GetInstance();
	MeteorPlayfield* meteorPlayfield = iCreator.CreateInstance<MeteorPlayfield>("MeteorPlayfield");

	meteorPlayfield->LazyConstruct();
	*/
	Playfield* newPlayfield = new RecordPlayfield();
	newPlayfield->SetLeaveGameFunction(leaveGame);
	newPlayfield->SetRestartGameFunction(restartGame);
	newPlayfield->SetEndGameFunction(endGame);

	return newPlayfield;
}

EventProcessor<Event>* RecordRulesetExecutor::getEventProcessor(Event * e)
{
	// 為什麼不用event自己來create? 因為要去搭配不同的mapper，所以要動態調配
	string processorType = GetProcessorType(e->GetTypeName()); // .c_str();

	LOG(LogLevel::Finer) << "InstantRulesetExecutor::getEventProcessor(Event*) : event [" << e->GetStartTime() << "] has processor type [" << processorType << "] from ["<< e->GetTypeName() << "].";


	//InstanceCreator<MtoObject>& iCreator = InstanceCreator<MtoObject>::GetInstance();
	//EventProcessor<Event>* eventProcessor = iCreator.CreateInstance<EventProcessor<Event>>(processorType);
	if (processorType == "StopSystemEventHandler") {
		// TODO: 在這邊把歌曲名稱擺進去
		return (new StopSystemEventHandler())->RegisterEvent(e);
	}
	else if (processorType == "EndSystemEventHandler") {
		// TODO: 在這邊把歌曲名稱擺進去
		return (new EndSystemEventHandler())->RegisterEvent(e);
	}
	else if (processorType == "RestartSystemEventHandler") {
		// TODO: 在這邊把歌曲名稱擺進去
		return (new RestartSystemEventHandler())->RegisterEvent(e);
	}


	LOG(LogLevel::Error) << "RecordRulesetExecutor::getEventProcessor() : [" << processorType << "] not found match.";
	throw runtime_error("RecordRulesetExecutor::getEventProcessor(Event*) : No matched processor type.");
	// TODO:吐錯誤訊息
	return nullptr;
}

string RecordRulesetExecutor::GetProcessorType(string eventType)
{
	map<string, string>::iterator iter = eventProcessorTable.find(eventType);
	if (iter != eventProcessorTable.end())
	{
		return eventProcessorTable[eventType];
	}


	// TODO: 吐一些錯誤訊息
	LOG(LogLevel::Error) << "RecordRulesetExecutor::GetProcessorType() : [" << eventType << "] no matched event processor.";
	return string("");
}

int RecordRulesetExecutor::playfieldLoad()
{
	RulesetExecutor<Event>::playfieldLoad();

	PeriodMap<EventProcessor<Event>*>* processors = playfield->GetEventProcessorMaster()->GetEventProcessorPeriods();

	LOG(LogLevel::Fine) << "RecordRulesetExecutor::playfieldLoad() : " << [](PeriodMap<EventProcessor<Event>*>* ps) {

		multimap<pair<float, float>, EventProcessor<Event>*>* periods = ps->GetPeriods();
		typename multimap<pair<float, float>, EventProcessor<Event>*>::iterator it;

		for (it = periods->begin(); it != periods->end(); it++) {
			LOG(LogLevel::Depricated) << "---timespan [" << it->first.first << "," << it->first.second << "] -> processor [" << it->second->GetStartTime() << "].";
		}


		return 0;
	}(processors);

	return 0;
}

