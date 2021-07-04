#ifndef RECORD_PITCH_BINDING_SET_H
#define RECORD_PITCH_BINDING_SET_H


#include "../../Instruments/Input/PitchBindingSet.h"
#include "../../Instruments/MeteoPianoPitchState.h"
#include "RecordInputManager.h"



using namespace std;
using namespace Instruments::Input;
using namespace Instruments;


namespace Record {
namespace Input {


	
	struct RecordPitchBindingSet : public TPitchBindingSet<RecordAction> {

		

	public:

		virtual int SetPitchBinding(Pitch pitch, RecordAction action);

		virtual RecordAction GetAction(Pitch pitch);

		virtual Pitch GetPitch(RecordAction action);

		int SwitchPitchState(MeteoPianoPitchState pState);

		MeteoPianoPitchState GetPitchState();

	protected:

		MeteoPianoPitchState pitchState = MeteoPianoPitchState::None;

		map<RecordAction, Pitch> loweredActionToPitch;
		map<Pitch, RecordAction> loweredPitchToAction;

		map<RecordAction, Pitch> raisedActionToPitch;
		map<Pitch, RecordAction> raisedPitchToAction;


	};
}}



#endif