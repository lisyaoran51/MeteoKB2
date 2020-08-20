#ifndef INSTANT_PLAYFIELD_H
#define INSTANT_PLAYFIELD_H

#include "../../../Games/Scene/Play/Playfield.h"
#include "../../../Util/MtoType.h"
#include "../../Config/InstantConfigManager.h"
#include "../../../Instruments/Pitch.h"
#include "../../../Framework/Input/KeyBindings/KeyBindingHandler.h"
#include "../../Input/InstantInputManager.h"
#include "../../../Instruments/MeteoPiano.h"
#include "../../Scheduler/Event/InstantEventProcessorMaster.h"



using namespace Util;
using namespace Games::Scenes::Play;
using namespace Games::Schedulers::Events;
using namespace Instant::Config;
using namespace Instruments;
using namespace Instant::Input;
using namespace Framework::Input::KeyBindings;
using namespace Instruments;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Instant::Schedulers::Events;



namespace Instant {
namespace Scenes{
namespace Play{

	class InstantPlayfield: public Playfield, public KeyBindingHandler<InstantAction> {

		int load();

		int load(FrameworkConfigManager* f, InstantConfigManager* m);

	public:

		InstantPlayfield();

		MapPitchShifter* GetMapPitchShifter();

		virtual int OnJudgement(HitObject * hitObject, Judgement * judgement);

		int SetIsGameControllingPitchState(bool value);

		int ChangePitchState(MeteoPianoPitchState s);

		MeteoPianoPitchState GetMeteoPianoPitchState();


		/* KeyBindingHandler<MeteorAction> */

		virtual int OnKeyDown(pair<InstantAction, int> action);

		virtual int OnKeyUp(InstantAction action);

		virtual int OnButtonDown(InstantAction action);

		virtual int OnButtonUp(InstantAction action);

		virtual int OnKnobTurn(pair<InstantAction, int> action);

		virtual int OnSlide(pair<InstantAction, int> action);

	protected:

		InstantEventProcessorMaster* instantEventProcessorMaster = nullptr;

		Pitch startPitch;

		int pitchCount;

		MTO_FLOAT explosionLifeTime = 0.2f;

		bool isGameControllingPitchState = true;

		MeteoPianoPitchState pitchState = MeteoPianoPitchState::None;

		/// <summary>
		/// 用來把map平移到想要的pitch的工具
		/// </summary>
		MapPitchShifter* mapPitchShifter = nullptr;

		virtual int LoadOnComplete();

		virtual EventProcessorMaster* createEventProcessorMaster();

		virtual DynamicEventGenerator* createDynamicEventGenerator();

	};

}}}



#endif