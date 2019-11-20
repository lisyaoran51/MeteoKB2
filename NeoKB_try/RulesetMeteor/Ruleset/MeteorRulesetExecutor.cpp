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
	LOG(LogLevel::Info) << "MeteorRulesetExecutor::load() : loading default meteor config.";

	MeteorConfigManager* meteorConfigManager = new MeteorConfigManager();
	meteorConfigManager->Initialize();

	meteorConfigManager->Set(MeteorSetting::WhiteKeyTargetHeight, 9);	// �C4	  ��9
	meteorConfigManager->Set(MeteorSetting::BlackKeyTargetHeight, 8);
	meteorConfigManager->Set(MeteorSetting::FallSpeed, MTO_FLOAT(8.f));	//���`16.f	�C12.f
	meteorConfigManager->Set(MeteorSetting::FallBrightness, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::FallLength, 1);
	meteorConfigManager->Set(MeteorSetting::ExplodeSpeed, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::ExplodeBrightness, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::GlowLineSpeed, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::GlowLineDuration, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::GlowLineBrightness, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::TargetLineBlinkSpeed, MTO_FLOAT(0.75f));
	meteorConfigManager->Set(MeteorSetting::TargetLineBrightness, MTO_FLOAT(0.05f));
	Cache<MeteorConfigManager>(meteorConfigManager);

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

