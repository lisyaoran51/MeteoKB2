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
	return 0;
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

	// record模式可以改pitch state
	if (!isGameControllingPitchState) {
		if (action == RecordAction::LowerOctave) {
			switch (pitchState) {
			case MeteoPianoPitchState::Lowered:
				ChangePitchState(MeteoPianoPitchState::Lowered);
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

EventProcessorMaster * RecordPlayfield::createEventProcessorMaster()
{
	return new RecordEventProcessorMaster();
}

DynamicEventGenerator * RecordPlayfield::createDynamicEventGenerator()
{
	return new DynamicEventGenerator(this);
}


