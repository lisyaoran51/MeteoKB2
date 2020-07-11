#include "MeteorRulesetExecutor.h"

#include "../Sheetmusic/MeteorSheetmusicConverter.h"
#include "../Sheetmusic/MeteorSheetmusicPostProcessor.h"
#include "../Play/MeteorPlayfield.h"
#include "../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "../Scheduler/Event/Effect/FallEffect.h"
#include "../Scheduler/Event/Effect/ExplodeEffect.h"
#include "../Scheduler/Event/Effect/GlowLineEffect.h"
#include "../Scheduler/Event/Effect/FallEffectMapper.h"
#include "../Scheduler/Event/Effect/EruptEffectMapper.h"
#include "../Scheduler/Event/Effect/ExplodeEffectMapper.h"
#include "../Scheduler/Event/Effect/GlowLineEffectMapper.h"
#include "../Scheduler/Event/Effect/TargetLineEffectMapper.h"
#include "../../Games/Scheduler/Event/SystemEvents/SystemEventHandler.h"
#include "../../Games/Scheduler/Event/SystemEvents/StopSystemEvent.h"
#include "../Input/MeteorInputManager.h"
#include "../Timing/MeteorTimeController.h"
#include "../../Framework/Timing/SpeedAdjusters/LinearSpeedAdjuster.h"
#include "Scoring/MeteorScoreProcessor.h"
#include "../Scheduler/Event/ControlPoints/MeteorNoteControlPointHitObject.h"
#include "../Scheduler/Event/IoEvents/SustainPedalLightRing.h"
#include "../Scheduler/Event/ControlPoints/MeteorInputKeyControlPointHitObject.h"
#include "../Scheduler/Event/InstrumentEvents/PianoEventProcessor.h"
#include "../Scheduler/Event/PlayfieldEvents/OctaveShiftEventProcessor.h"




using namespace Meteor::Rulesets;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::SystemEvents;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Games::Sheetmusics;
using namespace Meteor::Sheetmusics;
using namespace Meteor::Play;
using namespace Games::Schedulers::Events::Effects;
using namespace Meteor::Schedulers::Events::Effects;
using namespace Meteor::Input;
using namespace Meteor::Timing;
using namespace Framework::Timing::SpeedAdjusters;
using namespace Meteor::Rulesets::Scoring;
using namespace Meteor::Schedulers::Events::ControlPoints;
using namespace Meteor::Schedulers::Events::IoEvents;
using namespace Meteor::Schedulers::Events::InstrumentEvents;
using namespace Meteor::Schedulers::Events::PlayfieldEvents;






SmConverter * MeteorRulesetExecutor::createSmConverter(PatternGenerator * pg)
{
	return new MeteorSmConverter(pg);
}

SmPostprocessor * MeteorRulesetExecutor::createSmPostprocessor()
{
	return new MeteorSmPostprocessor();
}

int MeteorRulesetExecutor::load()
{
	
	MeteorTimeController* t = GetCache<MeteorTimeController>("TimeController");
	if (!t) {
		GetDependencies()->_DebugPrintCache();
		throw runtime_error("int MeteorRulesetExecutor::load() : MeteorTimeController not found in cache.");
	}
	// 讀config
	return 0;
}

int MeteorRulesetExecutor::load(MeteorTimeController * t)
{
	vector<Event*>* originalEvents = workingSm->GetSm()->GetEvents();
	vector<float> sectionTime;

	int section = -1;

	for (int i = 0; i < originalEvents->size(); i++) {
		if (dynamic_cast<PlayableControlPoint*>(originalEvents->at(i))) {

			PlayableControlPoint* playableControlPoint = dynamic_cast<PlayableControlPoint*>(originalEvents->at(i));

			if (playableControlPoint->GetSectionIndex() == -1) {
				if(sectionTime.size() != 0)
					throw runtime_error("int MeteorRulesetExecutor::load() : working sm section index error.");
				break;
			}
				
			/*
			 * TODO: 重要!! 這邊要先將events用時間排序，不然就是decoder要先排序一次，不然會出錯
			 */
			if (playableControlPoint->GetSectionIndex() > section) {
				for (int j = 0; i < playableControlPoint->GetSectionIndex() - section; j++) {
					sectionTime.push_back(playableControlPoint->GetStartTime());
				}
				section = playableControlPoint->GetSectionIndex();
			}
		}
	}
	
	t->SetSectionTime(&sectionTime);

	return 0;
}

MeteorRulesetExecutor::MeteorRulesetExecutor(): RegisterType("MeteorRulesetExecutor"), RulesetExecutor()
{
	// 如果要自定效果，要直接從config那裡改map algo，這邊不能動。
	eventProcessorTable["FallEffect"		] = "FallEffectMapper";
	eventProcessorTable["EruptEffect"		] = "EruptEffectMapper";
	eventProcessorTable["ExplodeEffect"		] = "ExplodeEffectMapper";
	eventProcessorTable["GlowLineEffect"	] = "GlowLineEffectMapper";
	eventProcessorTable["TargetLineEffect"	] = "TargetLineEffectMapper";
	eventProcessorTable["StopSystemEvent"	] = "SystemEventHandler";
	eventProcessorTable["NoteControlPoint"	] = "MeteorNoteControlPointHitObject";
	eventProcessorTable["InputKeyControlPoint"	] = "MeteorInputKeyControlPointHitObject";
	eventProcessorTable["SustainPedalIoEvent"	] = "SustainPedalLightRing";
	eventProcessorTable["PianoEvent"		] = "PianoEventProcessor";
	eventProcessorTable["OctaveShiftEvent"	] = "OctaveShiftEventProcessor";

	// 註冊private load (c++才需要)
	registerLoad(bind(static_cast<int(MeteorRulesetExecutor::*)(void)>(&MeteorRulesetExecutor::load), this));
	constructed = false;
}

int MeteorRulesetExecutor::LazyConstruct(WorkingSm * w, Ruleset* r)
{
	RulesetExecutor::LazyConstruct(w, r);
	constructed = true;
	return 0;
}

PassThroughInputManager * MeteorRulesetExecutor::CreateInputManager()
{
	return new MeteorInputManager(ruleset->GetRulesetInfo());
}

TimeController * MeteorRulesetExecutor::CreateTimeController()
{
	return new MeteorTimeController();
}

SpeedAdjuster * MeteorRulesetExecutor::CreateSpeedAdjuster()
{
	return new LinearSpeedAdjuster();
}

ScoreProcessor * MeteorRulesetExecutor::CreateScoreProcessor()
{
	return new MeteorScoreProcessor(this);
}

int MeteorRulesetExecutor::Elapse(MTO_FLOAT elapsedTime)
{
	// 之後可用來讀取輸入??先留著
	return 0;
}

Playfield* MeteorRulesetExecutor::createPlayfield()
{
	/* 不用搞那麼複雜，直接new一個就好
	InstanceCreator<MtoObject> iCreator = InstanceCreator<MtoObject>::GetInstance();
	MeteorPlayfield* meteorPlayfield = iCreator.CreateInstance<MeteorPlayfield>("MeteorPlayfield");

	meteorPlayfield->LazyConstruct();
	*/
	return new MeteorPlayfield();
}

EventProcessor<Event>* MeteorRulesetExecutor::getEventProcessor(Event * e)
{
	// 為什麼不用event自己來create? 因為要去搭配不同的mapper，所以要動態調配
	string processorType = GetProcessorType(e->GetTypeName()); // .c_str();

	LOG(LogLevel::Finer) << "MeteorRulesetExecutor::getEventProcessor(Event*) : event [" << e->GetStartTime() << "] has processor type [" << processorType << "] from ["<< e->GetTypeName() << "].";


	//InstanceCreator<MtoObject>& iCreator = InstanceCreator<MtoObject>::GetInstance();
	//EventProcessor<Event>* eventProcessor = iCreator.CreateInstance<EventProcessor<Event>>(processorType);
	if (processorType == "FallEffectMapper") {
		int width = playfield->GetWidth();
		int height = playfield->GetHeight();
		return (new FallEffectMapper(width, height))->RegisterEvent(e);
	}
	else if (processorType == "EruptEffectMapper") {
		int width = playfield->GetWidth();
		int height = playfield->GetHeight();
		return (new EruptEffectMapper(width, height))->RegisterEvent(e);
	}
	/* 這幾個效果不好，所以拿掉了
	//else if (processorType == "GlowLineEffectMapper") {
	//	int width = playfield->GetWidth();
	//	int height = playfield->GetHeight();
	//	return (new GlowLineEffectMapper(width, height))->RegisterEvent(e);
	//}
	//else if (processorType == "ExplodeEffectMapper") {
	//	int width = playfield->GetWidth();
	//	int height = playfield->GetHeight();
	//	return (new ExplodeEffectMapper(width, height))->RegisterEvent(e);
	//}
	//else if (processorType == "TargetLineEffectMapper") {
	//	int width = playfield->GetWidth();
	//	int height = playfield->GetHeight();
	//	return (new TargetLineEffectMapper(width, height))->RegisterEvent(e);
	//}
	*/
	else if (processorType == "SystemEventHandler") {
		// TODO: 在這邊把歌曲名稱擺進去
		return (new SystemEventHandler<StopSystemEvent>())->RegisterEvent(e);
	}
	else if (processorType == "MeteorNoteControlPointHitObject") {
		return (new MeteorNoteControlPointHitObject())->RegisterEvent(e);
	}
	else if (processorType == "SustainPedalLightRing") {
		LOG(LogLevel::Depricated) << "MeteorRulesetExecutor::getEventProcessor : getting event SustainPedalLightRing at [" << e->GetStartTime() << "]";
		return (new SustainPedalLightRing())->RegisterEvent(e);
	}
	else if (processorType == "MeteorInputKeyControlPointHitObject") {
		LOG(LogLevel::Depricated) << "MeteorRulesetExecutor::getEventProcessor : getting event MeteorInputKeyControlPointHitObject at [" << e->GetStartTime() << "]";
		return (new MeteorInputKeyControlPointHitObject())->RegisterEvent(e);
	}
	else if (processorType == "PianoEventProcessor") {
		LOG(LogLevel::Depricated) << "MeteorRulesetExecutor::getEventProcessor : getting event PianoEventProcessor at [" << e->GetStartTime() << "]";
		return (new PianoEventProcessor())->RegisterEvent(e);
	}
	else if (processorType == "OctaveShiftEventProcessor") {
		return (new OctaveShiftEventProcessor())->RegisterEvent(e);
	}


	LOG(LogLevel::Error) << "MeteorRulesetExecutor::getEventProcessor() : [" << processorType << "] not found match.";
	throw runtime_error("MeteorRulesetExecutor::getEventProcessor(Event*) : No matched processor type.");
	// TODO:吐錯誤訊息
	return NULL;
}

string MeteorRulesetExecutor::GetProcessorType(string eventType)
{
	map<string, string>::iterator iter = eventProcessorTable.find(eventType);
	if (iter != eventProcessorTable.end())
	{
		return eventProcessorTable[eventType];
	}


	// TODO: 吐一些錯誤訊息
	LOG(LogLevel::Error) << "MeteorRulesetExecutor::GetProcessorType() : [" << eventType << "] no matched event processor.";
	return string("");
}

int MeteorRulesetExecutor::playfieldLoad()
{
	RulesetExecutor<Event>::playfieldLoad();

	PeriodMap<EventProcessor<Event>*>* processors = playfield->GetEventProcessorMaster()->GetEventProcessorPeriods();

	LOG(LogLevel::Fine) << "MeteorRulesetExecutor::playfieldLoad() : " << [](PeriodMap<EventProcessor<Event>*>* ps) {

		multimap<pair<float, float>, EventProcessor<Event>*>* periods = ps->GetPeriods();
		typename multimap<pair<float, float>, EventProcessor<Event>*>::iterator it;

		for (it = periods->begin(); it != periods->end(); it++) {
			LOG(LogLevel::Debug) << "---timespan [" << it->first.first << "," << it->first.second << "] -> processor [" << it->second->GetStartTime() << "].";
		}


		return 0;
	}(processors);

	return 0;
}

