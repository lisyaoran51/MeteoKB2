#include "MeteorRulesetExecutor.h"

#include "../Sheetmusic/MeteorSheetmusicConverter.h"
#include "../Play/MeteorPlayfield.h"
#include "../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "../Scheduler/Event/Effect/FallEffect.h"
#include "../Scheduler/Event/Effect/ExplodeEffect.h"
#include "../Scheduler/Event/Effect/GlowLineEffect.h"
#include "../Scheduler/Event/Effect/FallEffectMapper.h"
#include "../Scheduler/Event/Effect/ExplodeEffectMapper.h"
#include "../Scheduler/Event/Effect/GlowLineEffectMapper.h"
#include "../Scheduler/Event/Effect/TargetLineEffectMapper.h"
#include "../../Games/Scheduler/Event/SystemEvents/SystemEventHandler.h"
#include "../../Games/Scheduler/Event/SystemEvents/StopSystemEvent.h"
#include "../Input/MeteorInputManager.h"
#include "../Timing/MeteorTimeController.h"
#include "../../Framework/Timing/SpeedAdjusters/LinearSpeedAdjuster.h"




using namespace Meteor::Rulesets;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::SystemEvents;
using namespace Games::Sheetmusics;
using namespace Meteor::Sheetmusics;
using namespace Meteor::Play;
using namespace Games::Schedulers::Events::Effects;
using namespace Meteor::Schedulers::Events::Effects;
using namespace Meteor::Input;
using namespace Meteor::Timing;
using namespace Framework::Timing::SpeedAdjusters;


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
	

	// Ūconfig
	return 0;
}

MeteorRulesetExecutor::MeteorRulesetExecutor(): RegisterType("MeteorRulesetExecutor"), RulesetExecutor()
{
	// �p�G�n�۩w�ĪG�A�n�����qconfig���̧�map algo�A�o�䤣��ʡC
	eventProcessorTable["FallEffect"		] = "FallEffectMapper";
	eventProcessorTable["ExplodeEffect"		] = "ExplodeEffectMapper";
	eventProcessorTable["GlowLineEffect"	] = "GlowLineEffectMapper";
	eventProcessorTable["TargetLineEffect"	] = "TargetLineEffectMapper";
	eventProcessorTable["StopSystemEvent"	] = "SystemEventHandler";

	// ���Uprivate load (c++�~�ݭn)
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

int MeteorRulesetExecutor::Elapse(MTO_FLOAT elapsedTime)
{
	// ����i�Ψ�Ū����J??���d��
	return 0;
}

Playfield* MeteorRulesetExecutor::createPlayfield()
{
	/* ���ηd��������A����new�@�ӴN�n
	InstanceCreator<MtoObject> iCreator = InstanceCreator<MtoObject>::GetInstance();
	MeteorPlayfield* meteorPlayfield = iCreator.CreateInstance<MeteorPlayfield>("MeteorPlayfield");

	meteorPlayfield->LazyConstruct();
	*/
	return new MeteorPlayfield();
}

EventProcessor<Event>* MeteorRulesetExecutor::getEventProcessor(Event * e)
{
	// �����򤣥�event�ۤv��create? �]���n�h�f�t���P��mapper�A�ҥH�n�ʺA�հt
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
		// TODO: �b�o���q���W���\�i�h
		return (new SystemEventHandler<StopSystemEvent>())->RegisterEvent(e);
	}

	throw runtime_error("MeteorRulesetExecutor::getEventProcessor(Event*) : No matched processor type.");
	// TODO:�R���~�T��
	return NULL;
}

string MeteorRulesetExecutor::GetProcessorType(string eventType)
{
	map<string, string>::iterator iter = eventProcessorTable.find(eventType);
	if (iter != eventProcessorTable.end())
	{
		return eventProcessorTable[eventType];
	}


	// TODO: �R�@�ǿ��~�T��
	return string("");
}

int MeteorRulesetExecutor::playfieldLoad()
{
	RulesetExecutor<Event>::playfieldLoad();

	PeriodMap<EventProcessor<Event>*>* processors = playfield->GetEventProcessorMaster()->GetEventProcessorPeriods();

	LOG(LogLevel::Debug) << "MeteorRulesetExecutor::playfieldLoad() : " << [](PeriodMap<EventProcessor<Event>*>* ps) {

		multimap<pair<float, float>, EventProcessor<Event>*>* periods = ps->GetPeriods();
		typename multimap<pair<float, float>, EventProcessor<Event>*>::iterator it;

		for (it = periods->begin(); it != periods->end(); it++) {
			LOG(LogLevel::Debug) << "---timespan [" << it->first.first << "," << it->first.second << "] -> processor [" << it->second->GetStartTime() << "].";
		}


		return 0;
	}(processors);

	return 0;
}

