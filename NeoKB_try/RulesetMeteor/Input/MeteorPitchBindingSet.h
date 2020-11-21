#ifndef METEOR_PITCH_BINDING_SET_H
#define METEOR_PITCH_BINDING_SET_H


#include "../../Instruments/Input/PitchBindingSet.h"
#include "../../Instruments/MeteoPianoPitchState.h"
#include "MeteorInputManager.h"



using namespace std;
using namespace Instruments::Input;
using namespace Instruments;


namespace Meteor {
namespace Input {


	
	struct MeteorPitchBindingSet : public TPitchBindingSet<MeteorAction> {

		

	public:

		virtual int SetPitchBinding(Pitch pitch, MeteorAction action);

		virtual MeteorAction GetAction(Pitch pitch);

		virtual Pitch GetPitch(MeteorAction action);

		int SwitchPitchState(MeteoPianoPitchState pState);

		MeteoPianoPitchState GetPitchState();

	protected:

		MeteoPianoPitchState pitchState = MeteoPianoPitchState::None;

		map<MeteorAction, Pitch> loweredActionToPitch;
		map<Pitch, MeteorAction> loweredPitchToAction;

		map<MeteorAction, Pitch> raisedActionToPitch;
		map<Pitch, MeteorAction> raisedPitchToAction;


	};
}}



#endif