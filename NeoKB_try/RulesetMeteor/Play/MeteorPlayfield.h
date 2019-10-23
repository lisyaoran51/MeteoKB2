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



using namespace Util;
using namespace Games::Scenes::Play;
using namespace Games::Schedulers::Events;
using namespace Meteor::Config;
using namespace Instruments;
using namespace Meteor::Input;
using namespace Framework::Input::KeyBindings;
using namespace Instruments;
using namespace Games::Schedulers::Events::Effects::Algorithms;



namespace Meteor {
namespace Play{

	class MeteorPlayfield: public Playfield, public KeyBindingHandler<MeteorAction> {

		int load();

		int load(FrameworkConfigManager* f, MeteorConfigManager* m);

	public:

		MeteorPlayfield();

		//virtual int Elapse(MTO_FLOAT elapsedTime);

		int SetIsGameControllingPitchState(bool value);

		int ChangePitchState(MeteoPianoPitchState s);


		/* KeyBindingHandler<MeteorAction> */

		virtual int OnKeyDown(pair<MeteorAction, int> action);

		virtual int OnKeyUp(MeteorAction action);

		virtual int OnButtonDown(MeteorAction action);

		virtual int OnButtonUp(MeteorAction action);

		virtual int OnKnobTurn(pair<MeteorAction, int> action);

		virtual int OnSlide(pair<MeteorAction, int> action);

	protected:

		Pitch startPitch;

		int pitchCount;



		bool isGameControllingPitchState = true;

		MeteoPianoPitchState pitchState = MeteoPianoPitchState::None;

		/// <summary>
		/// 用來把map平移到想要的pitch的工具
		/// </summary>
		MapPitchShifter* mapPitchShifter;

	};

}
}












#endif