#include "MeteorRulesetExecutor.h"

#include "../Sheetmusic/MeteorSheetmusicConverter.h"
#include "../Play/MeteorPlayfield.h"
#include "../../Base/Scheduler/Event/Effect/EffectMapper.h"
#include "../Scheduler/Event/Effect/FallEffect.h"
#include "../Scheduler/Event/Effect/ExplodeEffect.h"
#include "../Scheduler/Event/Effect/GlowLineEffect.h"
#include "../Scheduler/Event/Effect/FallEffectMapper.h"
#include "../Scheduler/Event/Effect/ExplodeEffectMapper.h"
#include "../Scheduler/Event/Effect/GlowLineEffectMapper.h"
#include "../Scheduler/Event/Effect/TargetLineEffectMapper.h"
#include "../../Base/Scheduler/Event/SystemEvents/SystemEventHandler.h"
#include "../../Base/Scheduler/Event/SystemEvents/StopSystemEvent.h"


using namespace Meteor::Rulesets;
using namespace Base::Schedulers::Events;
using namespace Base::Schedulers::Events::SystemEvents;
using namespace Base::Sheetmusics;
using namespace Meteor::Sheetmusics;
using namespace Meteor::Play;
using namespace Base::Schedulers::Events::Effects;
using namespace Meteor::Schedulers::Events::Effects;



SmConverter * MeteorRulesetExecutor::createSmConverter(PatternGenerator * pg)
{
	return new MeteorSmConverter(pg);
}

SmPostprocessor * MeteorRulesetExecutor::createSmPostprocessor()
{
	return new SmPostprocessor();
}

int MeteorRulesetExecutor::load()
{
	// 讀config
	return 0;
}

MeteorRulesetExecutor::MeteorRulesetExecutor(): RegisterType("MeteorRulesetExecutor"), RulesetExecutor()
{
	// 如果要自定效果，要直接從config那裡改map algo，這邊不能動。
	eventProcessorTable["FallEffect"		] = "FallEffectMapper";
	eventProcessorTable["ExplodeEffect"		] = "ExplodeEffectMapper";
	eventProcessorTable["GlowLineEffect"	] = "GlowLineEffectMapper";
	eventProcessorTable["TargetLineEffect"	] = "TargetLineEffectMapper";
	eventProcessorTable["StopSystemEvent"	] = "SystemEventHandler";

	// 註冊private load (c++才需要)
	registerLoad(bind(static_cast<int(MeteorRulesetExecutor::*)(void)>(&MeteorRulesetExecutor::load), this));
	constructed = false;
}

int MeteorRulesetExecutor::LazyConstruct(WorkingSm * w)
{
	RulesetExecutor::LazyConstruct(w);
	constructed = true;
	return 0;
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

	LOG(LogLevel::Finer) << "MeteorRulesetExecutor::getEventProcessor(Event*) : event [" << e->GetStartTime() << "] has processor type [" << processorType << "].";


	//InstanceCreator<MtoObject>& iCreator = InstanceCreator<MtoObject>::GetInstance();
	//EventProcessor<Event>* eventProcessor = iCreator.CreateInstance<EventProcessor<Event>>(processorType);
	if (processorType == "FallEffectMapper") {
		int width = playfield->GetWidth();
		int height = playfield->GetHeight();
		return (new FallEffectMapper(width, height))->RegisterEvent(e);
	}
	else if (processorType == "GlowLineEffectMapper") {
		int width = playfield->GetWidth();
		int height = playfield->GetHeight();
		return (new GlowLineEffectMapper(width, height))->RegisterEvent(e);
	}
	else if (processorType == "ExplodeEffectMapper") {
		int width = playfield->GetWidth();
		int height = playfield->GetHeight();
		return (new ExplodeEffectMapper(width, height))->RegisterEvent(e);
	}
	else if (processorType == "TargetLineEffectMapper") {
		int width = playfield->GetWidth();
		int height = playfield->GetHeight();
		return (new TargetLineEffectMapper(width, height))->RegisterEvent(e);
	}
	else if (processorType == "SystemEventHandler") {
		// TODO: 在這邊把歌曲名稱擺進去
		return (new SystemEventHandler<StopSystemEvent>())->RegisterEvent(e);
	}

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
	return string("");
}

