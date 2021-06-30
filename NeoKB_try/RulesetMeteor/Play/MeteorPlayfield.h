#ifndef METEOR_PLAYFIELD_H
#define METEOR_PLAYFIELD_H

#include "../../Games/Scene/Play/Playfield.h"
#include "../../Util/MtoType.h"
#include "../Config/MeteorConfigManager.h"
#include "../../Instruments/Pitch.h"
#include "../../Framework/Input/KeyBindings/KeyBindingHandler.h"
#include "../Input/MeteorInputManager.h"
#include "../../Instruments/MeteoPiano.h"
#include "../../Games/Scheduler/Event/Effect/Algorithm/MapPitchShifter.h"
#include "../Scheduler/Event/PlayfieldEvents/PlayfieldControllers/OctaveShifter.h"
#include "../../RulesetMeteor/Scheduler/Event/MeteorEventProcessorMaster.h"
#include "../Timing/MeteorTimeController.h"



using namespace Util;
using namespace Games::Scenes::Play;
using namespace Games::Schedulers::Events;
using namespace Meteor::Config;
using namespace Instruments;
using namespace Meteor::Input;
using namespace Framework::Input::KeyBindings;
using namespace Instruments;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Meteor::Schedulers::Events::PlayfieldEvents::PlayfieldControllers;
using namespace Meteor::Schedulers::Events;
using namespace Meteor::Timing;



namespace Meteor {
namespace Play{

	class MeteorPlayfield: public Playfield, public KeyBindingHandler<MeteorAction> {

		int load();

		int load(FrameworkConfigManager* f, MeteorConfigManager* m, TimeController* t);

	public:

		MeteorPlayfield();

		/// <summary>
		/// 紀錄event的最終時間，用來判斷遊戲結束時間
		/// </summary>
		virtual int Add(EventProcessor<Event>* ep);

		MapPitchShifter* GetMapPitchShifter();

		virtual int OnJudgement(HitObject * hitObject, Judgement * judgement);


		/* KeyBindingHandler<MeteorAction> */

		virtual int OnKeyDown(pair<MeteorAction, int> action);

		virtual int OnKeyUp(MeteorAction action);

		virtual int OnButtonDown(MeteorAction action);

		virtual int OnButtonUp(MeteorAction action);

		virtual int OnKnobTurn(pair<MeteorAction, int> action);

		virtual int OnSlide(pair<MeteorAction, int> action);

	protected:

		MeteorEventProcessorMaster* meteorEventProcessorMaster = nullptr;

		MeteorTimeController* meteorTimeController = nullptr;

		MTO_FLOAT explosionLifeTime = 0.2f;

		/// <summary>
		/// 用來把map平移到想要的pitch的工具
		/// </summary>
		MapPitchShifter* mapPitchShifter = nullptr;

		virtual EventProcessorMaster* createEventProcessorMaster();

		virtual DynamicEventGenerator* createDynamicEventGenerator();

	};

}
}












#endif