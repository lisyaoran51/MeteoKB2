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

	// TODO: �ھڦ��S������working sm�̭�audio file�M�w�n���nfilter sound event

	if (workingSm->GetTrack() == nullptr) {
		//eProcessorFilter->AddFilterCallback(...);
	}

	return 0;
}

RecordRulesetExecutor::RecordRulesetExecutor(): RegisterType("RecordRulesetExecutor"), RulesetExecutor()
{
	// �p�G�n�۩w�ĪG�A�n�����qconfig���̧�map algo�A�o�䤣��ʡC
	eventProcessorTable["StopSystemEvent"		] = "StopSystemEventHandler";
	eventProcessorTable["EndSystemEvent"		] = "EndSystemEventHandler";
	eventProcessorTable["RestartSystemEvent"	] = "RestartSystemEventHandler";

	// ���Uprivate load (c++�~�ݭn)
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
	// ����i�Ψ�Ū����J??���d��
	return 0;
}

Playfield* RecordRulesetExecutor::createPlayfield()
{
	/* ���ηd��������A����new�@�ӴN�n
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
	// �����򤣥�event�ۤv��create? �]���n�h�f�t���P��mapper�A�ҥH�n�ʺA�հt
	string processorType = GetProcessorType(e->GetTypeName()); // .c_str();

	LOG(LogLevel::Finer) << "InstantRulesetExecutor::getEventProcessor(Event*) : event [" << e->GetStartTime() << "] has processor type [" << processorType << "] from ["<< e->GetTypeName() << "].";


	//InstanceCreator<MtoObject>& iCreator = InstanceCreator<MtoObject>::GetInstance();
	//EventProcessor<Event>* eventProcessor = iCreator.CreateInstance<EventProcessor<Event>>(processorType);
	if (processorType == "StopSystemEventHandler") {
		// TODO: �b�o���q���W���\�i�h
		return (new StopSystemEventHandler())->RegisterEvent(e);
	}
	else if (processorType == "EndSystemEventHandler") {
		// TODO: �b�o���q���W���\�i�h
		return (new EndSystemEventHandler())->RegisterEvent(e);
	}
	else if (processorType == "RestartSystemEventHandler") {
		// TODO: �b�o���q���W���\�i�h
		return (new RestartSystemEventHandler())->RegisterEvent(e);
	}


	LOG(LogLevel::Error) << "RecordRulesetExecutor::getEventProcessor() : [" << processorType << "] not found match.";
	throw runtime_error("RecordRulesetExecutor::getEventProcessor(Event*) : No matched processor type.");
	// TODO:�R���~�T��
	return nullptr;
}

string RecordRulesetExecutor::GetProcessorType(string eventType)
{
	map<string, string>::iterator iter = eventProcessorTable.find(eventType);
	if (iter != eventProcessorTable.end())
	{
		return eventProcessorTable[eventType];
	}


	// TODO: �R�@�ǿ��~�T��
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

