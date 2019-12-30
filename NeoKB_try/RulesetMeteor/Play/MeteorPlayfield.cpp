#include "MeteorPlayfield.h"

#include "../../Games/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../RulesetMeteor/Scheduler/Event/Effect/Algorithm/ExplodeMapAlgorithm.h"
#include "../../RulesetMeteor/Scheduler/Event/Effect/Algorithm/FallMapAlgorithm.h"
#include "../../RulesetMeteor/Scheduler/Event/Effect/Algorithm/GlowLineMapAlgorithm.h"
#include "../../RulesetMeteor/Scheduler/Event/Effect/Algorithm/TargetLineMapAlgorithm.h"
#include "../../Util/Log.h"
#include "../../Games/Scheduler/Event/Effect/Algorithm/LinearMapPitchShifter.h"
#include "../../RulesetMeteor/Scheduler/Event/MeteorEventProcessorMaster.h"
#include "../../Games/Scheduler/Event/IoEvents/IoCommunicators/IoCommunicator.h"
#include "../Scheduler/Event/IoEvents/IoCommunicators/SustainPedalLightRingIoCommunicator.h"



using namespace Meteor::Play;
using namespace Meteor::Config;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Meteor::Schedulers::Events::Effects::Algorithms;
using namespace Util;
using namespace Meteor::Schedulers::Events;
using namespace Games::Schedulers::Events::IoEvents::IoCommunicators;
using namespace Meteor::Schedulers::Events::IoEvents::IoCommunicators;



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
	isGameControllingPitchState = false; // 暫時先這樣，Debug用


	if(f->Get(FrameworkSetting::StartPitch, (int*)&startPitch)){}
	if(f->Get(FrameworkSetting::Width, &pitchCount)){}

	if (!m->Get(MeteorSetting::ExplodeLifeTime, &explosionLifeTime)) {
		explosionLifeTime = 0.1f;
	}


	/* 利用map algo的名字建立map algo */
	InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
	string mapAlgoName;
	string ioCommunicatorName;

	/* --------------------- FallEffect map algo --------------------- */
	if (m->Get(MeteorSetting::FallMapAlgorithm, &mapAlgoName)) {
		MapAlgorithm<Event>* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithm<Event>>(mapAlgoName);

		mapAlgorithms["FallEffect"] = mapAlgo;
	}
	else
		mapAlgorithms["FallEffect"] = new FallMapAlgorithm();

	LOG(LogLevel::Finer) << "MeteorPlayfield::load() : FallMapAlgorithm [" << mapAlgorithms["FallEffect"]->GetTypeName() << "] loaded.";

	AddChild(mapAlgorithms["FallEffect"]);
	mapAlgorithms["FallEffect"]->RegisterBufferMap(bufferMap);

	/* --------------------- ExplodeEffect map algo --------------------- */
	if (m->Get(MeteorSetting::ExplodeMapAlgorithm, &mapAlgoName)) {
		MapAlgorithm<Event>* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithm<Event>>(mapAlgoName);

		mapAlgorithms["ExplodeEffect"] = mapAlgo;
	}
	else
		mapAlgorithms["ExplodeEffect"] = new ExplodeMapAlgorithm();

	LOG(LogLevel::Finer) << "MeteorPlayfield::load() : ExplodeMapAlgorithm [" << mapAlgorithms["ExplodeEffect"]->GetTypeName() << "] loaded.";

	AddChild(mapAlgorithms["ExplodeEffect"]);
	mapAlgorithms["ExplodeEffect"]->RegisterBufferMap(bufferMap);

	/* --------------------- GlowLineEffect map algo --------------------- */
	//if (m->Get(MeteorSetting::GlowLineMapAlgorithm, &mapAlgoName)) {
	//	MapAlgorithm<Event>* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithm<Event>>(mapAlgoName);
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
	//	MapAlgorithm<Event>* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithm<Event>>(mapAlgoName);
	//
	//	mapAlgorithms["TargetLineEffect"] = mapAlgo;
	//}
	//else
	//	mapAlgorithms["TargetLineEffect"] = new TargetLineMapAlgorithm();
	//
	//AddChild(mapAlgorithms["TargetLineEffect"]);
	//mapAlgorithms["TargetLineEffect"]->RegisterBufferMap(bufferMap);

	/* --------------------- Pedal event io communicator --------------------- */
	if (m->Get(MeteorSetting::SustainPedalLightRingIoCommunicator, &ioCommunicatorName)) {
		IoCommunicatorInterface* ioCommunicator = iCreator.CreateInstanceWithT<IoCommunicatorInterface>(ioCommunicatorName);
		LOG(LogLevel::Finer) << "MeteorPlayfield::load() : check";
		ioCommunicators["SustainPedalIoEvent"] = ioCommunicator;
	}
	else
		ioCommunicators["SustainPedalIoEvent"] = new SustainPedalLightRingIoCommunicator();

	LOG(LogLevel::Finer) << "MeteorPlayfield::load() : SustainPedalLightRingIoCommunicator [" << ioCommunicators["SustainPedalLightRingIoCommunicator"]->GetTypeName() << "] loaded.";

	AddChild(ioCommunicators["SustainPedalIoEvent"]);


	/*--------------------- map pitch shifter ---------------------*/
	string MapPitchShifterName;
	if (m->Get(MeteorSetting::MapPitchShifter, &MapPitchShifterName)) {
		mapPitchShifter = iCreator.CreateInstanceWithT<MapPitchShifter>(MapPitchShifterName);
	}
	else
		mapPitchShifter = new LinearMapPitchShifter();

	mapPitchShifter->LazyConstruct(&mapAlgorithms);
	AddChild(mapPitchShifter);

	



	return 0;
}

MeteorPlayfield::MeteorPlayfield(): Playfield(), RegisterType("MeteorPlayfield")
{
	// functional裡面的bind不能對overloading問題
	// https://stackoverflow.com/questions/4159487/stdbind-overload-resolution
	registerLoad(bind((int(MeteorPlayfield::*)())&MeteorPlayfield::load, this));
}

int MeteorPlayfield::OnJudgement(HitObject * hitObject, Judgement * judgement)
{
	LOG(LogLevel::Depricated) << "MeteorPlayfield::OnJudgement() : add judgement and try to create explosion." << judgement->GetIsHit();
	if (!judgement->GetIsHit())
		return -1;

	ExplodeEffect* effect = new ExplodeEffect(dynamic_cast<HasPitch*>(hitObject)->GetPitch(), 
											  0,
											  GetClock()->GetCurrentTime(),
											  explosionLifeTime);
	ExplodeEffectMapper* explosion = new ExplodeEffectMapper(GetWidth(), GetHeight(), hitObject);
	explosion->RegisterEvent(effect);
	//explosion->SetLifeTime(explosionLifeTime); // 這個應該要設定在map algo裡面，不該擺在這邊
	AddDynamic(explosion);


	LOG(LogLevel::Debug) << "MeteorPlayfield::OnJudgement() : create explosion.";

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
	if (s == MeteoPianoPitchState::Lowered) {
		pitchState = MeteoPianoPitchState::Lowered;
		mapPitchShifter->SetSeekPitch(Pitch::C1);
	}
	else if (s == MeteoPianoPitchState::None) {
		pitchState = MeteoPianoPitchState::None;
		mapPitchShifter->SetSeekPitch(Pitch::C);
	}
	else if (s == MeteoPianoPitchState::Raised) {
		pitchState = MeteoPianoPitchState::Raised;
		mapPitchShifter->SetSeekPitch(Pitch::c);
	}
	return 0;
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

EventProcessorMaster * MeteorPlayfield::createEventProcessorMaster()
{
	return new MeteorEventProcessorMaster();
}


