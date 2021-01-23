#include "PianoPitchBindingSet.h"


using namespace Instruments::Input;


int PianoPitchBindingSet::SetPitchBinding(Pitch pitch, PianoAction action)
{
	switch (pitchState) {

	case MeteoPianoPitchState::None:
		actionToPitch[action] = pitch;
		pitchToAction[pitch] = action;
		break;

	case MeteoPianoPitchState::Lowered:
		loweredActionToPitch[action] = pitch;
		loweredPitchToAction[pitch] = action;
		break;

	case MeteoPianoPitchState::Raised:
		raisedActionToPitch[action] = pitch;
		raisedPitchToAction[pitch] = action;
		break;

	}

	return 0;
}

PianoAction PianoPitchBindingSet::GetAction(Pitch pitch)
{
	map<Pitch, PianoAction>* actionMap = nullptr;

	switch (pitchState) {

	case MeteoPianoPitchState::None:
		actionMap = &pitchToAction;
		break;

	case MeteoPianoPitchState::Lowered:
		actionMap = &loweredPitchToAction;
		break;

	case MeteoPianoPitchState::Raised:
		actionMap = &raisedPitchToAction;
		break;

	}

	map<Pitch, PianoAction>::iterator iter;

	iter = actionMap->find(pitch);

	if (iter != actionMap->end())
		return actionMap->at(pitch);

	return PianoAction::None;
}

Pitch PianoPitchBindingSet::GetPitch(PianoAction action)
{
	map<PianoAction, Pitch>* pitchMap = nullptr;

	switch (pitchState) {

	case MeteoPianoPitchState::None:
		pitchMap = &actionToPitch;
		break;

	case MeteoPianoPitchState::Lowered:
		pitchMap = &loweredActionToPitch;
		break;

	case MeteoPianoPitchState::Raised:
		pitchMap = &raisedActionToPitch;
		break;

	}

	map<PianoAction, Pitch>::iterator iter;

	iter = pitchMap->find(action);

	if (iter != pitchMap->end())
		return pitchMap->at(action);

	return Pitch::None;
}

int PianoPitchBindingSet::SwitchPitchState(MeteoPianoPitchState pState)
{
	pitchState = pState;
	return 0;
}

MeteoPianoPitchState PianoPitchBindingSet::GetPitchState()
{
	return pitchState;
}
