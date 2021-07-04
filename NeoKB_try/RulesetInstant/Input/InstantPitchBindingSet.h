#ifndef INSTANT_PITCH_BINDING_SET_H
#define INSTANT_PITCH_BINDING_SET_H


#include "../../Instruments/Input/PitchBindingSet.h"
#include "../../Instruments/MeteoPianoPitchState.h"
#include "InstantInputManager.h"



using namespace std;
using namespace Instruments::Input;
using namespace Instruments;


namespace Instant {
namespace Input {


	
	struct InstantPitchBindingSet : public TPitchBindingSet<InstantAction> {

		

	public:

		virtual int SetPitchBinding(Pitch pitch, InstantAction action);

		virtual InstantAction GetAction(Pitch pitch);

		virtual Pitch GetPitch(InstantAction action);

		int SwitchPitchState(MeteoPianoPitchState pState);

		MeteoPianoPitchState GetPitchState();

	protected:

		MeteoPianoPitchState pitchState = MeteoPianoPitchState::None;

		map<InstantAction, Pitch> loweredActionToPitch;
		map<Pitch, InstantAction> loweredPitchToAction;

		map<InstantAction, Pitch> raisedActionToPitch;
		map<Pitch, InstantAction> raisedPitchToAction;


	};
}}



#endif