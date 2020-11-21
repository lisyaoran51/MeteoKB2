#include "MeteorPitchBindingSet.h"

using namespace Meteor::Input;


int MeteorPitchBindingSet::SetPitchBinding(Pitch pitch, MeteorAction action)
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

MeteorAction MeteorPitchBindingSet::GetAction(Pitch pitch)
{
	map<Pitch, MeteorAction>* actionMap = nullptr;

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

	map<Pitch, MeteorAction>::iterator iter;

	iter = actionMap->find(pitch);

	if (iter != actionMap->end())
		return actionMap->at(pitch);

	return MeteorAction::None;
}

Pitch MeteorPitchBindingSet::GetPitch(MeteorAction action)
{
	map<MeteorAction, Pitch>* pitchMap = nullptr;

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

	map<MeteorAction, Pitch>::iterator iter;

	iter = pitchMap->find(action);

	if (iter != pitchMap->end())
		return pitchMap->at(action);

	return Pitch::None;
}

int MeteorPitchBindingSet::SwitchPitchState(MeteoPianoPitchState pState)
{
	pitchState = pState;
	return 0;
}

MeteoPianoPitchState MeteorPitchBindingSet::GetPitchState()
{
	return pitchState;
}
