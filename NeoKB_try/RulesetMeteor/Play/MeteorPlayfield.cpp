#include "MeteorPlayfield.h"

#include "../../Games/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../RulesetMeteor/Scheduler/Event/Effect/Algorithm/ExplodeMapAlgorithm.h"
#include "../../RulesetMeteor/Scheduler/Event/Effect/Algorithm/FallMapAlgorithm.h"
#include "../../RulesetMeteor/Scheduler/Event/Effect/Algorithm/EruptMapAlgorithm.h"
#include "../../RulesetMeteor/Scheduler/Event/Effect/Algorithm/GlowLineMapAlgorithm.h"
#include "../../RulesetMeteor/Scheduler/Event/Effect/Algorithm/TargetLineMapAlgorithm.h"
#include "../../Util/Log.h"
#include "../../Games/Scheduler/Event/Effect/Algorithm/LinearMapPitchShifter.h"
#include "../../RulesetMeteor/Scheduler/Event/MeteorEventProcessorMaster.h"
#include "../../Games/Scheduler/Event/IoEvents/IoCommunicators/IoCommunicator.h"
#include "../Scheduler/Event/IoEvents/IoCommunicators/SustainPedalLightRingIoCommunicator.h"
#include "../Scheduler/Event/InstrumentEvents/InstrumentControllers/PianoController.h"
#include "../Scheduler/Event/InstrumentEvents/InstrumentControllers/VirtualPianoController.h"
#include "../Scheduler/Event/PlayfieldEvents/PlayfieldControllers/OctaveShifter.h"
#include "../Scheduler/Event/TimeEvents/TimeControllerControllers/RepeatPracticeController.h"



using namespace Meteor::Play;
using namespace Meteor::Config;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Meteor::Schedulers::Events::Effects::Algorithms;
using namespace Util;
using namespace Meteor::Schedulers::Events;
using namespace Games::Schedulers::Events::IoEvents::IoCommunicators;
using namespace Meteor::Schedulers::Events::IoEvents::IoCommunicators;
using namespace Meteor::Schedulers::Events::InstrumentEvents::InstrumentControllers;
using namespace Meteor::Schedulers::Events::PlayfieldEvents::PlayfieldControllers;
using namespace Meteor::Schedulers::Events::TimeEvents::TimeControllerControllers;


int MeteorPlayfield::load()
{
	LOG(LogLevel::Info) << "MeteorPlayfield::load() : Start loading game scene.";

	MeteorConfigManager* m = GetCache<MeteorConfigManager>("MeteorConfigManager");

	if (!m)
		throw runtime_error("int MeteorPlayfield::load() : MeteorConfigManager not found in cache.");

	FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");

	if (!f)
		throw runtime_error("int MeteorPlayfield::load() : FrameworkConfigManager not found in cache.");

	return load(f, m);
}

int MeteorPlayfield::load(FrameworkConfigManager* f, MeteorConfigManager * m)
{
	meteorEventProcessorMaster = dynamic_cast<MeteorEventProcessorMaster*>(eventProcessorMaster);

	if (meteorEventProcessorMaster == nullptr) {
		throw runtime_error("MeteorPlayfield::load() : meteorEventProcessorMaster is null.");
	}

	isGameControllingPitchState = false; // 暫時先這樣，Debug用
	

	if(f->Get(FrameworkSetting::StartPitch, (int*)&startPitch)){}
	if(f->Get(FrameworkSetting::Width, &pitchCount)){}

	if (!m->Get(MeteorSetting::ExplodeLifeTime, &explosionLifeTime)) {
		explosionLifeTime = 0.0f;
	}


	/* 利用map algo的名字建立map algo */
	InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
	string mapAlgoName;

	/* --------------------- FallEffect map algo --------------------- */
	if (m->Get(MeteorSetting::FallMapAlgorithm, &mapAlgoName)) {
		LOG(LogLevel::Finer) << "MeteorPlayfield::load() : finding fall effect map algorithm";
		MapAlgorithmInterface* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithmInterface>(mapAlgoName);

		LOG(LogLevel::Finer) << "MeteorPlayfield::load() : put into algorithms table";
		mapAlgorithms["FallEffect"] = mapAlgo;
	}
	else
		mapAlgorithms["FallEffect"] = new FallMapAlgorithm();
	LOG(LogLevel::Finer) << "MeteorPlayfield::load() : FallMapAlgorithm chosed" << mapAlgorithms["FallEffect"];

	LOG(LogLevel::Finer) << "MeteorPlayfield::load() : FallMapAlgorithm [" << mapAlgorithms["FallEffect"]->GetTypeName() << "] loaded.";

	AddChild(mapAlgorithms["FallEffect"]);
	mapAlgorithms["FallEffect"]->RegisterBufferMap(bufferMap);

	/* --------------------- EruptEffect map algo --------------------- */
	if (m->Get(MeteorSetting::EruptMapAlgorithm, &mapAlgoName)) {
		LOG(LogLevel::Finer) << "MeteorPlayfield::load() : finding erupt effect map algorithm";
		MapAlgorithmInterface* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithmInterface>(mapAlgoName);

		LOG(LogLevel::Finer) << "MeteorPlayfield::load() : put into algorithms table";
		mapAlgorithms["EruptEffect"] = mapAlgo;
	}
	else
		mapAlgorithms["EruptEffect"] = new EruptMapAlgorithm();
	LOG(LogLevel::Finer) << "MeteorPlayfield::load() : EruptMapAlgorithm chosed" << mapAlgorithms["EruptEffect"];

	LOG(LogLevel::Finer) << "MeteorPlayfield::load() : EruptMapAlgorithm [" << mapAlgorithms["EruptEffect"]->GetTypeName() << "] loaded.";

	AddChild(mapAlgorithms["EruptEffect"]);
	mapAlgorithms["EruptEffect"]->RegisterBufferMap(bufferMap);

	/* --------------------- ExplodeEffect map algo --------------------- */
	if (m->Get(MeteorSetting::ExplodeMapAlgorithm, &mapAlgoName)) {
		MapAlgorithmInterface* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithmInterface>(mapAlgoName);

		mapAlgorithms["ExplodeEffect"] = mapAlgo;
	}
	else
		mapAlgorithms["ExplodeEffect"] = new ExplodeMapAlgorithm();

	LOG(LogLevel::Finer) << "MeteorPlayfield::load() : ExplodeMapAlgorithm [" << mapAlgorithms["ExplodeEffect"]->GetTypeName() << "] loaded.";

	AddChild(mapAlgorithms["ExplodeEffect"]);
	mapAlgorithms["ExplodeEffect"]->RegisterBufferMap(bufferMap);

	/* --------------------- GlowLineEffect map algo --------------------- */
	//if (m->Get(MeteorSetting::GlowLineMapAlgorithm, &mapAlgoName)) {
	//	MapAlgorithmInterface* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithmInterface>(mapAlgoName);
	//
	//	mapAlgorithms["GlowLineEffect"] = mapAlgo;
	//}
	//else
	//	mapAlgorithms["GlowLineEffect"] = new GlowLineMapAlgorithm();
	//
	//LOG(LogLevel::Finer) << "MeteorPlayfield::load() : GlowLineMapAlgorithm [" << mapAlgorithms["GlowLineEffect"]->GetTypeName() << "] loaded.";
	//
	//AddChild(mapAlgorithms["GlowLineEffect"]);
	//mapAlgorithms["GlowLineEffect"]->RegisterBufferMap(bufferMap);

	/* --------------------- TargetLineEffect map algo --------------------- */
	//if (m->Get(MeteorSetting::TargetLineMapAlgorithm, &mapAlgoName)) {
	//	MapAlgorithmInterface* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithmInterface>(mapAlgoName);
	//
	//	mapAlgorithms["TargetLineEffect"] = mapAlgo;
	//}
	//else
	//	mapAlgorithms["TargetLineEffect"] = new TargetLineMapAlgorithm();
	//
	//AddChild(mapAlgorithms["TargetLineEffect"]);
	//mapAlgorithms["TargetLineEffect"]->RegisterBufferMap(bufferMap);

	/* --------------------- Pedal event io communicator --------------------- */
	string ioCommunicatorName;
	if (m->Get(MeteorSetting::SustainPedalLightRingIoCommunicator, &ioCommunicatorName)) {
		IoCommunicatorInterface* ioCommunicator = iCreator.CreateInstanceWithT<IoCommunicatorInterface>(ioCommunicatorName);

		ioCommunicators["SustainPedalIoEvent"] = ioCommunicator;
	}
	else {
		ioCommunicators["SustainPedalIoEvent"] = new SustainPedalLightRingIoCommunicator();
	}

	LOG(LogLevel::Finer) << "MeteorPlayfield::load() : SustainPedalLightRingIoCommunicator [" << ioCommunicators["SustainPedalIoEvent"]->GetTypeName() << "] loaded.";

	AddChild(ioCommunicators["SustainPedalIoEvent"]);

	/* --------------------- Piano Controller --------------------- */
	string instrumentControllerName;
	if (m->Get(MeteorSetting::InstrumentController, &instrumentControllerName)) {
		InstrumentControllerInterface* instrumentController = iCreator.CreateInstanceWithT<InstrumentControllerInterface>(instrumentControllerName);

		instrumentControllers["PianoEvent"] = instrumentController;
	}
	else {
		instrumentControllers["PianoEvent"] = new PianoController();
	}
	AddChild(instrumentControllers["PianoEvent"]);


	if (m->Get(MeteorSetting::VirtualInstrumentController, &instrumentControllerName)) {
		InstrumentControllerInterface* instrumentController = iCreator.CreateInstanceWithT<InstrumentControllerInterface>(instrumentControllerName);

		instrumentControllers["PianoSoundEvent"] = instrumentController;
	}
	else {
		instrumentControllers["PianoSoundEvent"] = new VirtualPianoController();
	}
	AddChild(instrumentControllers["PianoSoundEvent"]);

	/*--------------------- map pitch shifter ---------------------*/
	string MapPitchShifterName;
	if (m->Get(MeteorSetting::MapPitchShifter, &MapPitchShifterName)) {
		mapPitchShifter = iCreator.CreateInstanceWithT<MapPitchShifter>(MapPitchShifterName);
	}
	else
		mapPitchShifter = new LinearMapPitchShifter();

	mapPitchShifter->LazyConstruct(&mapAlgorithms);
	AddChild(mapPitchShifter);

	/*--------------------- Octave shifter ---------------------*/
	string octaveShifterName;
	if (m->Get(MeteorSetting::OctaveShifter, &octaveShifterName)) {
		playfieldControllers["OctaveShiftEvent"] = iCreator.CreateInstanceWithT<OctaveShifter>(octaveShifterName);
	}
	else
		playfieldControllers["OctaveShiftEvent"] = new OctaveShifter();

	playfieldControllers["OctaveShiftEvent"]->LazyConstruct(this);
	AddChild(playfieldControllers["OctaveShiftEvent"]);


	/*--------------------- repeat practice controller ---------------------*/
	string repeatPracticeControllerName;
	if (m->Get(MeteorSetting::RepeatPracticeController, &repeatPracticeControllerName)) {
		timeControllerControllers["RepeatPracticeEvent"] = iCreator.CreateInstanceWithT<TimeControllerControllerInterface>(repeatPracticeControllerName);
	}
	else
		timeControllerControllers["RepeatPracticeEvent"] = new RepeatPracticeController();
	AddChild(timeControllerControllers["RepeatPracticeEvent"]);



	/* --------------------- System Controller --------------------- */
	string systemControllerName;

	if (m->Get(MeteorSetting::SystemController, &systemControllerName)) {
		SystemController* systemController = iCreator.CreateInstanceWithT<SystemController>(systemControllerName);
		systemController->LazyConstruct(leaveGame, restartGame, endGame);
		systemControllers["StopSystemEvent"] = systemController;
		systemControllers["RestartSystemEvent"] = systemController;
	}
	else {
		systemControllers["StopSystemEvent"] = new SystemController();
		systemControllers["RestartSystemEvent"] = systemControllers["StopSystemEvent"];
		systemControllers["StopSystemEvent"]->LazyConstruct(leaveGame, restartGame, endGame);
	}
	AddChild(systemControllers["StopSystemEvent"]);

	return 0;
}

MeteorPlayfield::MeteorPlayfield(): Playfield(), RegisterType("MeteorPlayfield")
{
	// functional裡面的bind不能對overloading問題
	// https://stackoverflow.com/questions/4159487/stdbind-overload-resolution
	registerLoad(bind((int(MeteorPlayfield::*)())&MeteorPlayfield::load, this));
}

MapPitchShifter * MeteorPlayfield::GetMapPitchShifter()
{
	return mapPitchShifter;
}

int MeteorPlayfield::OnJudgement(HitObject * hitObject, Judgement * judgement)
{
	LOG(LogLevel::Depricated) << "MeteorPlayfield::OnJudgement() : add judgement and try to create explosion." << judgement->GetIsHit();
	if (!judgement->GetIsHit())
		return -1;

	// 因為爆破效果不好，所以不用了
	return 0;

	ExplodeEffect* effect = new ExplodeEffect(dynamic_cast<HasPitch*>(hitObject)->GetPitch(), 
											  0,
											  GetClock()->GetCurrentTime(),
											  explosionLifeTime);
	ExplodeEffectMapper* explosion = new ExplodeEffectMapper(GetWidth(), GetHeight(), hitObject);
	explosion->RegisterEvent(effect);
	//explosion->SetLifeTime(explosionLifeTime); // 這個應該要設定在map algo裡面，不該擺在這邊
	AddDynamic(explosion);


	LOG(LogLevel::Debug) << "MeteorPlayfield::OnJudgement() : create explosion on [" << (int)dynamic_cast<HasPitch*>(hitObject)->GetPitch() << "].";

	// 傳送分數?
	return 0;
}

int MeteorPlayfield::SetIsGameControllingPitchState(bool value)
{
	isGameControllingPitchState = value;
	return 0;
}

int MeteorPlayfield::ChangePitchState(MeteoPianoPitchState s)
{
	//if (!isGameControllingPitchState)
	//	return 0;

	if (s == MeteoPianoPitchState::Lowered) {
		pitchState = MeteoPianoPitchState::Lowered;
		meteorEventProcessorMaster->ChangePitchState(MeteoPianoPitchState::Lowered);
	}
	else if (s == MeteoPianoPitchState::None) {
		pitchState = MeteoPianoPitchState::None;
		meteorEventProcessorMaster->ChangePitchState(MeteoPianoPitchState::None);
	}
	else if (s == MeteoPianoPitchState::Raised) {
		pitchState = MeteoPianoPitchState::Raised;
		meteorEventProcessorMaster->ChangePitchState(MeteoPianoPitchState::Raised);
	}
	return 0;
}

MeteoPianoPitchState MeteorPlayfield::GetMeteoPianoPitchState()
{
	return pitchState;
}

int MeteorPlayfield::OnKeyDown(pair<MeteorAction, int> action)
{
	return 0;
}

int MeteorPlayfield::OnKeyUp(MeteorAction action)
{
	return 0;
}

int MeteorPlayfield::OnButtonDown(MeteorAction action)
{
	LOG(LogLevel::Debug) << "MeteorPlayfield::OnButtonDown() : button = " << int(action) << ".";

	if (!isGameControllingPitchState) {
		if (action == MeteorAction::LowerOctave) {
			switch (pitchState) {
			case MeteoPianoPitchState::Lowered:
				break;
			case MeteoPianoPitchState::None:
				pitchState = MeteoPianoPitchState::Lowered;
				mapPitchShifter->SetSeekPitch(Pitch::C1);
				break;
			case MeteoPianoPitchState::Raised:
				pitchState = MeteoPianoPitchState::None;
				mapPitchShifter->SetSeekPitch(Pitch::C);
				break;
			}
		}
		else if (action == MeteorAction::RaiseOctave) {
			switch (pitchState) {
			case MeteoPianoPitchState::Lowered:
				pitchState = MeteoPianoPitchState::None;
				mapPitchShifter->SetSeekPitch(Pitch::C);
				break;
			case MeteoPianoPitchState::None:
				pitchState = MeteoPianoPitchState::Raised;
				mapPitchShifter->SetSeekPitch(Pitch::c);
				break;
			case MeteoPianoPitchState::Raised:
				break;
			}
		}
	}
	return 0;
}

int MeteorPlayfield::OnButtonUp(MeteorAction action)
{
	return 0;
}

int MeteorPlayfield::OnKnobTurn(pair<MeteorAction, int> action)
{
	return 0;
}

int MeteorPlayfield::OnSlide(pair<MeteorAction, int> action)
{
	return 0;
}

int MeteorPlayfield::LoadOnComplete()
{
	ChangePitchState(MeteoPianoPitchState::None);
	return 0;
}

EventProcessorMaster * MeteorPlayfield::createEventProcessorMaster()
{
	return new MeteorEventProcessorMaster();
}

DynamicEventGenerator * MeteorPlayfield::createDynamicEventGenerator()
{
	return new DynamicEventGenerator(this);
}


