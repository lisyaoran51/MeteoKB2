#include "RecordPlayfield.h"

#include "../../../Games/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../../Util/Log.h"
#include "../../Scheduler/Event/RecordEventProcessorMaster.h"
#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"



using namespace Record::Scenes::Play;
using namespace Record::Config;
using namespace Util;
using namespace Record::Schedulers::Events;
using namespace Games::Output::Bluetooths;


int RecordPlayfield::load()
{
	LOG(LogLevel::Info) << "RecordPlayfield::load() : Start loading game scene.";

	RecordConfigManager* m = GetCache<RecordConfigManager>("RecordConfigManager");

	if (!m)
		throw runtime_error("int RecordPlayfield::load() : InstantConfigManager not found in cache.");

	FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");

	if (!f)
		throw runtime_error("int RecordPlayfield::load() : FrameworkConfigManager not found in cache.");

	return load(f, m);
}

int RecordPlayfield::load(FrameworkConfigManager* f, RecordConfigManager * m)
{
	recordEventProcessorMaster = dynamic_cast<RecordEventProcessorMaster*>(eventProcessorMaster);

	if (recordEventProcessorMaster == nullptr) {
		throw runtime_error("RecordPlayfield::load() : RecordEventProcessorMaster is null.");
	}

	isGameControllingPitchState = false; // 暫時先這樣，Debug用
	

	if(f->Get(FrameworkSetting::StartPitch, (int*)&startPitch)){}
	if(f->Get(FrameworkSetting::Width, &pitchCount)){}


	/* 利用map algo的名字建立map algo */
	InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
	string mapAlgoName;


	/* --------------------- System Controller --------------------- */
	string systemControllerName;

	if (m->Get(RecordSetting::RecordSystemController, &systemControllerName)) {
		SystemController* systemController = iCreator.CreateInstanceWithT<SystemController>(systemControllerName);
		systemController->LazyConstruct(leaveGame, restartGame, endGame);
		systemControllers["StopSystemEvent"] = systemController;
		systemControllers["RestartSystemEvent"] = systemController;
		systemControllers["EndSystemEvent"] = systemController;
	}
	else {
		systemControllers["StopSystemEvent"] = new SystemController();
		systemControllers["RestartSystemEvent"] = systemControllers["StopSystemEvent"];
		systemControllers["EndSystemEvent"] = systemControllers["StopSystemEvent"];
		systemControllers["StopSystemEvent"]->LazyConstruct(leaveGame, restartGame, endGame);
	}
	AddChild(systemControllers["StopSystemEvent"]);

	return 0;
}

RecordPlayfield::RecordPlayfield(): Playfield(), RegisterType("RecordPlayfield")
{
	// functional裡面的bind不能對overloading問題
	// https://stackoverflow.com/questions/4159487/stdbind-overload-resolution
	registerLoad(bind((int(RecordPlayfield::*)())&RecordPlayfield::load, this));
}

int RecordPlayfield::OnJudgement(HitObject * hitObject, Judgement * judgement)
{
	LOG(LogLevel::Depricated) << "MeteorPlayfield::OnJudgement() : add judgement and try to create explosion." << judgement->GetIsHit();
	if (!judgement->GetIsHit())
		return -1;

	// 因為爆破效果不好，所以不用了
	return 0;
}

int RecordPlayfield::SetIsGameControllingPitchState(bool value)
{
	isGameControllingPitchState = value;
	return 0;
}

int RecordPlayfield::ChangePitchState(MeteoPianoPitchState s)
{
	//if (!isGameControllingPitchState)
	//	return 0;

	if (s == MeteoPianoPitchState::Lowered) {
		pitchState = MeteoPianoPitchState::Lowered;
		recordEventProcessorMaster->ChangePitchState(MeteoPianoPitchState::Lowered);
	}
	else if (s == MeteoPianoPitchState::None) {
		pitchState = MeteoPianoPitchState::None;
		recordEventProcessorMaster->ChangePitchState(MeteoPianoPitchState::None);
	}
	else if (s == MeteoPianoPitchState::Raised) {
		pitchState = MeteoPianoPitchState::Raised;
		recordEventProcessorMaster->ChangePitchState(MeteoPianoPitchState::Raised);
	}
	return 0;
}

MeteoPianoPitchState RecordPlayfield::GetMeteoPianoPitchState()
{
	return pitchState;
}

int RecordPlayfield::OnKeyDown(pair<RecordAction, int> action)
{

	return 0;
}

int RecordPlayfield::OnKeyUp(RecordAction action)
{
	return 0;
}

int RecordPlayfield::OnButtonDown(RecordAction action)
{
	LOG(LogLevel::Debug) << "RecordPlayfield::OnButtonDown() : button = " << int(action) << ".";

	// Instant模式應該改成不能夠change pitch state
	if (!isGameControllingPitchState) {
		if (action == RecordAction::LowerOctave) {
			switch (pitchState) {
			case MeteoPianoPitchState::Lowered:
				break;
			case MeteoPianoPitchState::None:
				pitchState = MeteoPianoPitchState::Lowered;
				//mapPitchShifter->SetSeekPitch(Pitch::C1);
				break;
			case MeteoPianoPitchState::Raised:
				pitchState = MeteoPianoPitchState::None;
				//mapPitchShifter->SetSeekPitch(Pitch::C);
				break;
			}
		}
		else if (action == RecordAction::RaiseOctave) {
			switch (pitchState) {
			case MeteoPianoPitchState::Lowered:
				pitchState = MeteoPianoPitchState::None;
				//mapPitchShifter->SetSeekPitch(Pitch::C);
				break;
			case MeteoPianoPitchState::None:
				pitchState = MeteoPianoPitchState::Raised;
				//mapPitchShifter->SetSeekPitch(Pitch::c);
				break;
			case MeteoPianoPitchState::Raised:
				break;
			}
		}
	}
	return 0;
}

int RecordPlayfield::OnButtonUp(RecordAction action)
{
	return 0;
}

int RecordPlayfield::OnKnobTurn(pair<RecordAction, int> action)
{
	return 0;
}

int RecordPlayfield::OnSlide(pair<RecordAction, int> action)
{
	return 0;
}

int RecordPlayfield::LoadOnComplete()
{
	ChangePitchState(MeteoPianoPitchState::None);
	return 0;
}

EventProcessorMaster * RecordPlayfield::createEventProcessorMaster()
{
	return new RecordEventProcessorMaster();
}

DynamicEventGenerator * RecordPlayfield::createDynamicEventGenerator()
{
	return new DynamicEventGenerator(this);
}


