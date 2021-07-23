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
#include "../../Games/Scheduler/Event/ControlPoints/SectionStartControlPoint.h"
#include "../Input/MeteorInputManager.h"
#include "../Timing/MeteorTimeController.h"
#include "../../Framework/Timing/SpeedAdjusters/LinearSpeedAdjuster.h"
#include "Scoring/MeteorScoreProcessor.h"
#include "../Scheduler/Event/ControlPoints/MeteorNoteControlPointHitObject.h"
#include "../Scheduler/Event/IoEvents/SustainPedalLightRing.h"
#include "../Scheduler/Event/ControlPoints/MeteorInputKeyControlPointHitObject.h"
#include "../Scheduler/Event/InstrumentEvents/PianoEventProcessor.h"
#include "../Scheduler/Event/InstrumentEvents/PianoSoundEventProcessor.h"
#include "../Scheduler/Event/PlayfieldEvents/OctaveShiftEventProcessor.h"
#include "../Scheduler/Event/TimeEvents/RepeatPracticeEventProcessor.h"
#include "../Scheduler/Event/RecorderEvents/MeteorButtonRecorderEventProcessor.h"
#include "Replays/MeteorReplayRecorder.h"
#include "../Scenes/Results/MeteorResult.h"




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
using namespace Meteor::Schedulers::Events::TimeEvents;
using namespace Meteor::Rulesets::Replays;
using namespace Meteor::Scenes::Results;
using namespace Meteor::Schedulers::Events::RecorderEvents;






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

	Instrument* i = GetCache<Instrument>("Instrument");
	if (!i) {
		throw runtime_error("int MeteorRulesetExecutor::load() : Instrument not found in cache.");
	}

	ReplayRecorder* r = GetCache<ReplayRecorder>("ReplayRecorder");
	if (!r) {
		throw runtime_error("int MeteorRulesetExecutor::load() : ReplayRecorder not found in cache.");
	}

	VolumeController* v = GetCache<VolumeController>("VolumeController");
	if (!v) {
		throw runtime_error("int MeteorRulesetExecutor::load() : VolumeController not found in cache.");
	}
	// Ūconfig
	return load(t, i, r, v);
}

int MeteorRulesetExecutor::load(MeteorTimeController * t, Instrument* i, ReplayRecorder* r, VolumeController* v)
{

	LOG(LogLevel::Info) << "MeteorRulesetExecutor::load() : computing section time.";

	compositeMeteoPiano = dynamic_cast<CompositeMeteoPiano*>(i);
	compositeMeteoPiano->ChangePitchState(MeteoPianoPitchState::None);
	compositeMeteoPiano->SetGameControllingPitchState(true);

	/* �ھڳo���Ц��S����O��T�M�w�n���n�ιq�������O�C */
	if (workingSm->GetSm()->GetSmInfo()->hasPedalData) {

		LOG(LogLevel::Debug) << "MeteorRulesetExecutor::load() : this song has pedal data.";

		/* �p�G���۽�O�A�N�@�ߤ���game control sustain */
		if (compositeMeteoPiano->GetSustainType() != SustainType::SustainPedal) {
			LOG(LogLevel::Debug) << "MeteorRulesetExecutor::load() : set sustain type to game control.";

			compositeMeteoPiano->ChangeSustainType(SustainType::GameControllingSustain);
			r->SetGameControllingSustainPedal(true);
		}
		else
			LOG(LogLevel::Debug) << "MeteorRulesetExecutor::load() : this song has no pedal data.";


		compositeMeteoPiano->GetVirtualMeteoPiano()->SetVirtualMeteoPianoSustainType(VirtualMeteoPianoSustainType::Pedal);
	}
	else {
		LOG(LogLevel::Debug) << "MeteorRulesetExecutor::load() : set sustain type to auto.";
		compositeMeteoPiano->ChangeSustainType(SustainType::AutoSustain);
		r->SetGameControllingSustainPedal(false);
		compositeMeteoPiano->GetVirtualMeteoPiano()->SetVirtualMeteoPianoSustainType(VirtualMeteoPianoSustainType::Auto);
	}

	// �o�䮳�쪺Event�w�g�Q�B�z�L�F�A�S��section�]�Q�[�n�F
	vector<Event*>* processedEvents = sm->GetEvents();
	vector<float> sectionTime;

	LOG(LogLevel::Fine) << "MeteorRulesetExecutor::load() : event size [" << processedEvents->size() << "].";

	
	for (int i = 0; i < processedEvents->size(); i++) {
		if (dynamic_cast<SectionStartControlPoint*>(processedEvents->at(i))) {

			sectionTime.push_back(processedEvents->at(i)->GetStartTime());
		}
	}
	sort(sectionTime.begin(), sectionTime.end());

	for(int i = 0; i < sectionTime.size(); i++)
		LOG(LogLevel::Depricated) << "MeteorRulesetExecutor::load() : section [" << i << "] [" << sectionTime[i] << "].";

	t->SetSectionTime(&sectionTime);

	volumeController = v;

	volumeController->SetSampleVolumeRatio(0.8);

	volumeController->SetMirrorSampleVolumeRatio(0.6);


	LOG(LogLevel::Finer) << "MeteorRulesetExecutor::load() : end.";
	return 0;
}

MeteorRulesetExecutor::MeteorRulesetExecutor(): RegisterType("MeteorRulesetExecutor"), RulesetExecutor()
{
	// �p�G�n�۩w�ĪG�A�n�����qconfig���̧�map algo�A�o�䤣��ʡC
	eventProcessorTable["FallEffect"				] = "FallEffectMapper";
	eventProcessorTable["EruptEffect"				] = "EruptEffectMapper";
	eventProcessorTable["ExplodeEffect"				] = "ExplodeEffectMapper";
	eventProcessorTable["GlowLineEffect"			] = "GlowLineEffectMapper";
	eventProcessorTable["TargetLineEffect"			] = "TargetLineEffectMapper";
	eventProcessorTable["StopSystemEvent"			] = "SystemEventHandler";
	eventProcessorTable["NoteControlPoint"			] = "MeteorNoteControlPointHitObject";
	eventProcessorTable["InputKeyControlPoint"		] = "MeteorInputKeyControlPointHitObject";
	eventProcessorTable["SustainPedalIoEvent"		] = "SustainPedalLightRing";
	eventProcessorTable["RepeatPracticeEvent"		] = "RepeatPracticeEventProcessor";
	eventProcessorTable["PianoEvent"				] = "PianoEventProcessor";
	eventProcessorTable["PianoSoundEvent"			] = "PianoSoundEventProcessor";
	eventProcessorTable["OctaveShiftEvent"			] = "OctaveShiftEventProcessor";
	//eventProcessorTable["MeteorButtonRecorderEvent"	] = "MeteorButtonRecorderEventProcessor";

	// ���Uprivate load (c++�~�ݭn)
	registerLoad(bind(static_cast<int(MeteorRulesetExecutor::*)(void)>(&MeteorRulesetExecutor::load), this));
	constructed = false;
}

MeteorRulesetExecutor::~MeteorRulesetExecutor()
{
	LOG(LogLevel::Debug) << "MeteorRulesetExecutor::~MeteorRulesetExecutor() : turn off game control sustain." << (int)compositeMeteoPiano->GetSustainType();
	compositeMeteoPiano->SetGameControllingPitchState(false);
	compositeMeteoPiano->ChangePitchState(MeteoPianoPitchState::None);
	if(compositeMeteoPiano->GetSustainType() == SustainType::GameControllingSustain)
		compositeMeteoPiano->ChangeSustainType(SustainType::AutoSustain);


	volumeController->SetSampleVolumeRatio(1);

	volumeController->SetMirrorSampleVolumeRatio(1);
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
	MeteorTimeController* t = new MeteorTimeController();

	t->AddOnPause(this, [=]() {

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::HardwareGameEvent);

		json context;
		context["Events"].push_back(string("Pause,1"));

		meteoContextBluetoothMessage->SetContextInJson(context);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);

		return 0;
	}, "Lambda_MeteorRulesetExecutor::HandleOnPause");

	t->AddOnPauseEnd(this, [=]() {

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::HardwareGameEvent);

		json context;
		context["Events"].push_back(string("Pause,-1"));

		meteoContextBluetoothMessage->SetContextInJson(context);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);

		return 0;
	}, "Lambda_MeteorRulesetExecutor::HandleOnPauseEnd");


	return t;
}

SpeedAdjuster * MeteorRulesetExecutor::CreateSpeedAdjuster()
{
	return new LinearSpeedAdjuster();
}

ScoreProcessor * MeteorRulesetExecutor::CreateScoreProcessor()
{
	return new MeteorScoreProcessor(this);
}

ReplayRecorder * MeteorRulesetExecutor::CreateReplayRecorder()
{
	LOG(LogLevel::Debug) << "MeteorRulesetExecutor::CreateReplayRecorder() : create new recorder.";

	return new MeteorReplayRecorder();
}

Result * MeteorRulesetExecutor::CreateResult(Score* s)
{
	MeteorResult* meteorResult = new MeteorResult(s);

	

	return meteorResult;
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

	Playfield* newPlayfield = new MeteorPlayfield();
	newPlayfield->SetLeaveGameFunction(leaveGame);
	newPlayfield->SetRestartGameFunction(restartGame);
	newPlayfield->SetEndGameFunction(endGame);


	return newPlayfield;
}

EventProcessor<Event>* MeteorRulesetExecutor::getEventProcessor(Event * e)
{
	// �����򤣥�event�ۤv��create? �]���n�h�f�t���P��mapper�A�ҥH�n�ʺA�հt
	string processorType = GetProcessorType(e->GetTypeName()); // .c_str();

	if (processorType == "") {
		LOG(LogLevel::Finer) << "MeteorRulesetExecutor::getEventProcessor(Event*) : event [" << e->GetStartTime() << "] has no processor type from [" << e->GetTypeName() << "].";
		return nullptr;
	}
	LOG(LogLevel::Depricated) << "MeteorRulesetExecutor::getEventProcessor(Event*) : event [" << e->GetStartTime() << "] has processor type [" << processorType << "] from ["<< e->GetTypeName() << "].";


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
	/* �o�X�ӮĪG���n�A�ҥH�����F
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
		// TODO: �b�o���q���W���\�i�h
		return (new SystemEventHandler<StopSystemEvent>())->RegisterEvent(e);
	}
	else if (processorType == "MeteorNoteControlPointHitObject") {
		return (new MeteorNoteControlPointHitObject())->RegisterEvent(e);
	}
	else if (processorType == "SustainPedalLightRing") {
		LOG(LogLevel::Depricated) << "MeteorRulesetExecutor::getEventProcessor : getting event SustainPedalLightRing at [" << e->GetStartTime() << "]";
		return (new SustainPedalLightRing())->RegisterEvent(e);
	}
	else if (processorType == "RepeatPracticeEventProcessor") {
		LOG(LogLevel::Depricated) << "MeteorRulesetExecutor::getEventProcessor : getting event RepeatPracticeEventProcessor at [" << e->GetStartTime() << "]";
		return (new RepeatPracticeEventProcessor())->RegisterEvent(e);
	}
	else if (processorType == "MeteorInputKeyControlPointHitObject") {
		LOG(LogLevel::Depricated) << "MeteorRulesetExecutor::getEventProcessor : getting event MeteorInputKeyControlPointHitObject at [" << e->GetStartTime() << "]";
		return (new MeteorInputKeyControlPointHitObject())->RegisterEvent(e);
		// score processor�|�۰ʧ�input key control point���p���R��
	}
	else if (processorType == "PianoEventProcessor") {
		LOG(LogLevel::Depricated) << "MeteorRulesetExecutor::getEventProcessor : getting event PianoEventProcessor at [" << e->GetStartTime() << "]";
		return (new PianoEventProcessor())->RegisterEvent(e);
	}
	else if (processorType == "PianoSoundEventProcessor") {
		LOG(LogLevel::Depricated) << "MeteorRulesetExecutor::getEventProcessor : getting event PianoEventProcessor at [" << e->GetStartTime() << "]";
		return (new PianoSoundEventProcessor())->RegisterEvent(e);
	}
	else if (processorType == "OctaveShiftEventProcessor") {
		return (new OctaveShiftEventProcessor())->RegisterEvent(e);
	}
	else if (processorType == "MeteorButtonRecorderEventProcessor") {
		return (new MeteorButtonRecorderEventProcessor())->RegisterEvent(e);
	}


	LOG(LogLevel::Error) << "MeteorRulesetExecutor::getEventProcessor() : [" << processorType << "] not found match.";
	throw runtime_error("MeteorRulesetExecutor::getEventProcessor(Event*) : No matched processor type.");
	// TODO:�R���~�T��
	return nullptr;
}

string MeteorRulesetExecutor::GetProcessorType(string eventType)
{
	map<string, string>::iterator iter = eventProcessorTable.find(eventType);
	if (iter != eventProcessorTable.end())
	{
		return eventProcessorTable[eventType];
	}


	// TODO: �R�@�ǿ��~�T��
	LOG(LogLevel::Warning) << "MeteorRulesetExecutor::GetProcessorType() : [" << eventType << "] no matched event processor.";
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
			LOG(LogLevel::Fine) << "---timespan [" << it->first.first << "," << it->first.second << "] -> processor [" << it->second->GetStartTime() << "].";
		}


		return 0;
	}(processors);

	return 0;
}

