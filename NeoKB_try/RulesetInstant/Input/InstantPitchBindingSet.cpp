#include "InstantPitchBindingSet.h"

using namespace Instant::Input;


int InstantPitchBindingSet::SetPitchBinding(Pitch pitch, InstantAction action)
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

InstantAction InstantPitchBindingSet::GetAction(Pitch pitch)
{
	map<Pitch, InstantAction>* actionMap = nullptr;

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

	map<Pitch, InstantAction>::iterator iter;

	iter = actionMap->find(pitch);

	if (iter != actionMap->end())
		return actionMap->at(pitch);

	return InstantAction::None;
}

Pitch InstantPitchBindingSet::GetPitch(InstantAction action)
{
	map<InstantAction, Pitch>* pitchMap = nullptr;

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

	map<InstantAction, Pitch>::iterator iter;

	iter = pitchMap->find(action);

	if (iter != pitchMap->end())
		return pitchMap->at(action);

	return Pitch::None;
}

int InstantPitchBindingSet::SwitchPitchState(MeteoPianoPitchState pState)
{
	pitchState = pState;
	return 0;
}

MeteoPianoPitchState InstantPitchBindingSet::GetPitchState()
{
	return pitchState;
}
