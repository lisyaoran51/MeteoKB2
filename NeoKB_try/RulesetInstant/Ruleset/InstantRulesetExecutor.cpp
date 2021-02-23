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

	// TODO: �ھڦ��S������working sm�̭�audio file�M�w�n���nfilter sound event

	if (workingSm->GetTrack() == nullptr) {
		//eProcessorFilter->AddFilterCallback(...);
	}

	return 0;
}

InstantRulesetExecutor::InstantRulesetExecutor(): RegisterType("InstantRulesetExecutor"), RulesetExecutor()
{
	// �p�G�n�۩w�ĪG�A�n�����qconfig���̧�map algo�A�o�䤣��ʡC
	eventProcessorTable["InstantFallEffect"		] = "InstantFallEffectMapper";
	eventProcessorTable["InstantGlowLineEffect"	] = "InstantGlowLineEffectMapper";
	eventProcessorTable["StopSystemEvent"		] = "SystemEventHandler";
	eventProcessorTable["InstantPianoSoundEvent"] = "InstantPianoSoundEventProcessor";

	// ���Uprivate load (c++�~�ݭn)
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
	// ����i�Ψ�Ū����J??���d��
	return 0;
}

Playfield* InstantRulesetExecutor::createPlayfield()
{
	/* ���ηd��������A����new�@�ӴN�n
	InstanceCreator<MtoObject> iCreator = InstanceCreator<MtoObject>::GetInstance();
	MeteorPlayfield* meteorPlayfield = iCreator.CreateInstance<MeteorPlayfield>("MeteorPlayfield");

	meteorPlayfield->LazyConstruct();
	*/
	return new InstantPlayfield()->SetLeaveGameFunction(leaveGame)->SetRestartGameFunction(restartGame);
}

EventProcessor<Event>* InstantRulesetExecutor::getEventProcessor(Event * e)
{
	// �����򤣥�event�ۤv��create? �]���n�h�f�t���P��mapper�A�ҥH�n�ʺA�հt
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
		// TODO: �b�o���q���W���\�i�h
		return (new SystemEventHandler<SystemEvent>())->RegisterEvent(e);
	}


	LOG(LogLevel::Error) << "InstantRulesetExecutor::getEventProcessor() : [" << processorType << "] not found match.";
	throw runtime_error("InstantRulesetExecutor::getEventProcessor(Event*) : No matched processor type.");
	// TODO:�R���~�T��
	return nullptr;
}

string InstantRulesetExecutor::GetProcessorType(string eventType)
{
	map<string, string>::iterator iter = eventProcessorTable.find(eventType);
	if (iter != eventProcessorTable.end())
	{
		return eventProcessorTable[eventType];
	}


	// TODO: �R�@�ǿ��~�T��
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

