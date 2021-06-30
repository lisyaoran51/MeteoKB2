#include "InstantPlayfield.h"

#include "../../../Games/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../Scheduler/Event/Effect/Algorithm/InstantFallMapAlgorithm.h"
#include "../../Scheduler/Event/Effect/Algorithm/InstantGlowLineMapAlgorithm.h"
#include "../../Scheduler/Event/InstrumentEvents/InstrumentControllers/InstantVirtualPianoController.h"
#include "../../../Util/Log.h"
#include "../../../Games/Scheduler/Event/Effect/Algorithm/LinearMapPitchShifter.h"
#include "../../Scheduler/Event/InstantEventProcessorMaster.h"
#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Scheduler/Event/InstantDynamicEventGenerator.h"



using namespace Instant::Scenes::Play;
using namespace Instant::Config;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Instant::Schedulers::Events::Effects::Algorithms;
using namespace Instant::Schedulers::Events::InstrumentEvents::InstrumentControllers;
using namespace Util;
using namespace Instant::Schedulers::Events;
using namespace Games::Output::Bluetooths;


int InstantPlayfield::load()
{
	LOG(LogLevel::Info) << "InstantPlayfield::load() : Start loading game scene.";

	InstantConfigManager* m = GetCache<InstantConfigManager>("InstantConfigManager");

	if (!m)
		throw runtime_error("int InstantPlayfield::load() : InstantConfigManager not found in cache.");

	FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");

	if (!f)
		throw runtime_error("int InstantPlayfield::load() : FrameworkConfigManager not found in cache.");

	return load(f, m);
}

int InstantPlayfield::load(FrameworkConfigManager* f, InstantConfigManager * m)
{
	instantEventProcessorMaster = dynamic_cast<InstantEventProcessorMaster*>(eventProcessorMaster);

	if (instantEventProcessorMaster == nullptr) {
		throw runtime_error("InstantPlayfield::load() : InstantEventProcessorMaster is null.");
	}

	isGameControllingPitchState = false; // 暫時先這樣，Debug用
	

	if(f->Get(FrameworkSetting::StartPitch, (int*)&startPitch)){}
	if(f->Get(FrameworkSetting::Width, &pitchCount)){}


	/* 利用map algo的名字建立map algo */
	InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
	string mapAlgoName;

	/* --------------------- InstantFallEffect map algo --------------------- */
	if (m->Get(InstantSetting::InstantFallMapAlgorithm, &mapAlgoName)) {
		LOG(LogLevel::Finer) << "InstantPlayfield::load() : finding fall effect map algorithm";
		MapAlgorithmInterface* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithmInterface>(mapAlgoName);

		LOG(LogLevel::Finer) << "InstantPlayfield::load() : put into algorithms table";
		mapAlgorithms["InstantFallEffect"] = mapAlgo;
	}
	else
		mapAlgorithms["InstantFallEffect"] = new InstantFallMapAlgorithm();
	dynamic_cast<InstantFallMapAlgorithm*>(mapAlgorithms["InstantFallEffect"])->SetPlayfield(this);
		

	LOG(LogLevel::Finer) << "InstantPlayfield::load() : FallMapAlgorithm chosed" << mapAlgorithms["InstantFallEffect"];

	LOG(LogLevel::Finer) << "InstantPlayfield::load() : FallMapAlgorithm [" << mapAlgorithms["InstantFallEffect"]->GetTypeName() << "] loaded.";

	AddChild(mapAlgorithms["InstantFallEffect"]);
	mapAlgorithms["InstantFallEffect"]->RegisterBufferMap(bufferMap);

	/* --------------------- InstantGlowLineEffect map algo --------------------- */
	if (m->Get(InstantSetting::InstantGlowLineMapAlgorithm, &mapAlgoName)) {
		LOG(LogLevel::Finer) << "InstantPlayfield::load() : finding fall effect map algorithm";
		MapAlgorithmInterface* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithmInterface>(mapAlgoName);

		LOG(LogLevel::Finer) << "InstantPlayfield::load() : put into algorithms table";
		mapAlgorithms["InstantGlowLineEffect"] = mapAlgo;
	}
	else
		mapAlgorithms["InstantGlowLineEffect"] = new InstantGlowLineMapAlgorithm();
	LOG(LogLevel::Finer) << "InstantPlayfield::load() : InstantGlowLineMapAlgorithm chosed" << mapAlgorithms["InstantGlowLineEffect"];

	LOG(LogLevel::Finer) << "InstantPlayfield::load() : InstantGlowLineMapAlgorithm [" << mapAlgorithms["InstantGlowLineEffect"]->GetTypeName() << "] loaded.";

	AddChild(mapAlgorithms["InstantGlowLineEffect"]);
	mapAlgorithms["InstantGlowLineEffect"]->RegisterBufferMap(bufferMap);

	/* --------------------- Piano Controller --------------------- */
	string instrumentControllerName;

	if (m->Get(InstantSetting::InstantVirtualInstrumentController, &instrumentControllerName)) {
		InstrumentControllerInterface* instrumentController = iCreator.CreateInstanceWithT<InstrumentControllerInterface>(instrumentControllerName);

		instrumentControllers["InstantPianoSoundEvent"] = instrumentController;
	}
	else {
		instrumentControllers["InstantPianoSoundEvent"] = new InstantVirtualPianoController();
	}
	AddChild(instrumentControllers["InstantPianoSoundEvent"]);

	return 0;
}

InstantPlayfield::InstantPlayfield(): Playfield(), RegisterType("InstantPlayfield")
{
	// functional裡面的bind不能對overloading問題
	// https://stackoverflow.com/questions/4159487/stdbind-overload-resolution
	registerLoad(bind((int(InstantPlayfield::*)())&InstantPlayfield::load, this));
}

int InstantPlayfield::OnJudgement(HitObject * hitObject, Judgement * judgement)
{
	LOG(LogLevel::Depricated) << "MeteorPlayfield::OnJudgement() : add judgement and try to create explosion." << judgement->GetIsHit();
	if (!judgement->GetIsHit())
		return -1;

	// 因為爆破效果不好，所以不用了
	return 0;
}

int InstantPlayfield::SetWorkingSm(WorkingSm * sm)
{
	workingSm = sm;
	return 0;
}

WorkingSm * InstantPlayfield::GetWorkingSm()
{
	return workingSm;
}

int InstantPlayfield::OnKeyDown(pair<InstantAction, int> action)
{

	return 0;
}

int InstantPlayfield::OnKeyUp(InstantAction action)
{
	return 0;
}

int InstantPlayfield::OnButtonDown(InstantAction action)
{
	LOG(LogLevel::Debug) << "InstantPlayfield::OnButtonDown() : button = " << int(action) << ".";

	// Instant模式應該改成不能夠change pitch state
	if (!isGameControllingPitchState) {
		if (action == InstantAction::LowerOctave) {
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
		else if (action == InstantAction::RaiseOctave) {
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

int InstantPlayfield::OnButtonUp(InstantAction action)
{
	return 0;
}

int InstantPlayfield::OnKnobTurn(pair<InstantAction, int> action)
{
	return 0;
}

int InstantPlayfield::OnSlide(pair<InstantAction, int> action)
{
	return 0;
}

EventProcessorMaster * InstantPlayfield::createEventProcessorMaster()
{
	return new InstantEventProcessorMaster();
}

DynamicEventGenerator * InstantPlayfield::createDynamicEventGenerator()
{
	return new InstantDynamicEventGenerator(this);
}

int InstantPlayfield::onMessage(MeteoBluetoothMessage * message)
{
	if (message->GetCommand() == MeteoCommand::AppGameEvent) {

		MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);
		json context = contextMessage->GetContextInJson();
		
		if (context.contains("Events") == 0) {
			LOG(LogLevel::Warning) << "InstantPlayfield::onMessage() : format error.";
			return -1;
		}

		if (context["Events"].size() == 0) {
			LOG(LogLevel::Warning) << "InstantPlayfield::onMessage() : not event inside.";
			return -1;
		}

		string gameEventContext = context["Events"][0];
		vector<string> parameters = StringSplitter::Split(gameEventContext, ",");

		if (parameters.size() == 0)
			return -1;

		if (parameters[0] == "ShiftOctave") {

			if (parameters.size() < 2)
				return -1;

			// -1: 低八度 0:正常八度 1:高八度
			int state = stoi(parameters[1]);

			if (state < -1 || state > 1)
				return -1;

			switch (state)
			{
			case -1:
				ChangePitchState(MeteoPianoPitchState::Lowered);
				break;
			case 0:
				ChangePitchState(MeteoPianoPitchState::None);
				break;
			case 1:
				ChangePitchState(MeteoPianoPitchState::Raised);
				break;
			}

		}
	}

	return 0;
}


