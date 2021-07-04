#include "RecordPitchBindingSet.h"

using namespace Record::Input;


int RecordPitchBindingSet::SetPitchBinding(Pitch pitch, RecordAction action)
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

RecordAction RecordPitchBindingSet::GetAction(Pitch pitch)
{
	map<Pitch, RecordAction>* actionMap = nullptr;

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

	map<Pitch, RecordAction>::iterator iter;

	iter = actionMap->find(pitch);

	if (iter != actionMap->end())
		return actionMap->at(pitch);

	return RecordAction::None;
}

Pitch RecordPitchBindingSet::GetPitch(RecordAction action)
{
	map<RecordAction, Pitch>* pitchMap = nullptr;

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

	map<RecordAction, Pitch>::iterator iter;

	iter = pitchMap->find(action);

	if (iter != pitchMap->end())
		return pitchMap->at(action);

	return Pitch::None;
}

int RecordPitchBindingSet::SwitchPitchState(MeteoPianoPitchState pState)
{
	pitchState = pState;
	return 0;
}

MeteoPianoPitchState RecordPitchBindingSet::GetPitchState()
{
	return pitchState;
}
