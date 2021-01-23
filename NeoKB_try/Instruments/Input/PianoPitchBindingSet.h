#ifndef PIANO_PITCH_BINDING_SET_H
#define PIANO_PITCH_BINDING_SET_H


#include "PitchBindingSet.h"
#include "PianoAction.h"
#include "../MeteoPianoPitchState.h"


using namespace Instruments;


namespace Instruments {
namespace Input {

	class PianoPitchBindingSet : public TPitchBindingSet<PianoAction> {

	public:

		virtual int SetPitchBinding(Pitch pitch, PianoAction action);

		virtual PianoAction GetAction(Pitch pitch);

		virtual Pitch GetPitch(PianoAction action);

		int SwitchPitchState(MeteoPianoPitchState pState);

		MeteoPianoPitchState GetPitchState();

	protected:

		MeteoPianoPitchState pitchState = MeteoPianoPitchState::None;

		map<PianoAction, Pitch> loweredActionToPitch;
		map<Pitch, PianoAction> loweredPitchToAction;

		map<PianoAction, Pitch> raisedActionToPitch;
		map<Pitch, PianoAction> raisedPitchToAction;

	};




}}



#endif