#ifndef RECORD_PLAYFIELD_H
#define RECORD_PLAYFIELD_H

#include "../../../Games/Scene/Play/Playfield.h"
#include "../../../Util/MtoType.h"
#include "../../Config/RecordConfigManager.h"
#include "../../../Instruments/Pitch.h"
#include "../../../Framework/Input/KeyBindings/KeyBindingHandler.h"
#include "../../Input/RecordInputManager.h"
#include "../../../Instruments/MeteoPiano.h"
#include "../../Scheduler/Event/RecordEventProcessorMaster.h"



using namespace Util;
using namespace Games::Scenes::Play;
using namespace Games::Schedulers::Events;
using namespace Record::Config;
using namespace Instruments;
using namespace Record::Input;
using namespace Framework::Input::KeyBindings;
using namespace Instruments;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Record::Schedulers::Events;



namespace Record {
namespace Scenes{
namespace Play{

	class RecordPlayfield: public Playfield, public KeyBindingHandler<RecordAction> {

		int load();

		int load(FrameworkConfigManager* f, RecordConfigManager* m);

	public:

		RecordPlayfield();

		virtual int OnJudgement(HitObject * hitObject, Judgement * judgement);

		int SetIsGameControllingPitchState(bool value);

		int ChangePitchState(MeteoPianoPitchState s);

		MeteoPianoPitchState GetMeteoPianoPitchState();


		/* KeyBindingHandler<MeteorAction> */

		virtual int OnKeyDown(pair<RecordAction, int> action);

		virtual int OnKeyUp(RecordAction action);

		virtual int OnButtonDown(RecordAction action);

		virtual int OnButtonUp(RecordAction action);

		virtual int OnKnobTurn(pair<RecordAction, int> action);

		virtual int OnSlide(pair<RecordAction, int> action);

	protected:

		RecordEventProcessorMaster* recordEventProcessorMaster = nullptr;

		Pitch startPitch;

		int pitchCount;

		MTO_FLOAT explosionLifeTime = 0.2f;

		bool isGameControllingPitchState = true;

		MeteoPianoPitchState pitchState = MeteoPianoPitchState::None;

		/// <summary>
		/// 用來把map平移到想要的pitch的工具
		/// </summary>
		//MapPitchShifter* mapPitchShifter = nullptr;

		virtual int LoadOnComplete();

		virtual EventProcessorMaster* createEventProcessorMaster();

		virtual DynamicEventGenerator* createDynamicEventGenerator();

	};

}}}



#endif