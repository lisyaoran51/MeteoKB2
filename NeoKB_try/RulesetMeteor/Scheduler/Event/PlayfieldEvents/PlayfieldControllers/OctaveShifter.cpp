#include "OctaveShifter.h"

#include "../../../../Play/MeteorPlayfield.h"
#include "../../../../../Util/Log.h"


using namespace Meteor::Schedulers::Events::PlayfieldEvents::PlayfieldControllers;
using namespace Meteor::Play;
using namespace Util;


OctaveShifter::OctaveShifter(): RegisterType("OctaveShifter")
{
}

int OctaveShifter::LazyConstruct(Playfield * p)
{
	PlayfieldController::LazyConstruct(p);

	meteorPlayfield = dynamic_cast<MeteorPlayfield*>(playfield);

	return 0;
}

int OctaveShifter::implementControlPlayfield(EventProcessor<Event>* eProcessor)
{
	LOG(LogLevel::Debug) << "OctaveShifter::implementControlPlayfield() : shift octave.";
	OctaveShiftEventProcessor* octaveShiftEventProcessor = dynamic_cast<OctaveShiftEventProcessor*>(eProcessor);

	/*	平移八度已經被改掉了
	mapPitchShifter = dynamic_cast<MeteorPlayfield*>(playfield)->GetMapPitchShifter();
	if (mapPitchShifter == nullptr)
		return 0;

	double framesPerSecond = GetClock()->GetFramesPerSecond();

	float shiftTime = octaveShiftEventProcessor->GetShiftTimeLength();

	if (shiftTime == -1) {
		mapPitchShifter->SetSeekSpeed(framesPerSecond);
	}
	else if (shiftTime == 0) {
		mapPitchShifter->SetFloatSeekSpeed(12.0, 0.1);
	}
	else
		mapPitchShifter->SetFloatSeekSpeed(12.0, shiftTime);

	*/

#pragma region (Drepricated)改用浮動的平移速度，不然有bug
	/* 改用浮動的平移速度，不然有bug
	if (shiftTime == -1) {
		mapPitchShifter->SetSeekSpeed(framesPerSecond);
	}
	else if (shiftTime == 0) {
		mapPitchShifter->SetSeekSpeed(12.0 * framesPerSecond);
	}
	else
		mapPitchShifter->SetSeekSpeed(12.0 / shiftTime);
	*/
#pragma endregion


	if (octaveShiftEventProcessor->GetShiftType() == OctaveShiftType::Lower) {
		switch (meteorPlayfield->GetMeteoPianoPitchState()) {
		case MeteoPianoPitchState::Lowered:
			break;
		case MeteoPianoPitchState::None:
			meteorPlayfield->ChangePitchState(MeteoPianoPitchState::Lowered);
			//mapPitchShifter->SetSeekPitch(Pitch::C1);
			break;
		case MeteoPianoPitchState::Raised:
			meteorPlayfield->ChangePitchState(MeteoPianoPitchState::None);
			//mapPitchShifter->SetSeekPitch(Pitch::C);
			break;
		}
	}
	else if(octaveShiftEventProcessor->GetShiftType() == OctaveShiftType::Raise) {
		switch (meteorPlayfield->GetMeteoPianoPitchState()) {
		case MeteoPianoPitchState::Lowered:
			meteorPlayfield->ChangePitchState(MeteoPianoPitchState::None);
			//mapPitchShifter->SetSeekPitch(Pitch::C);
			break;
		case MeteoPianoPitchState::None:
			meteorPlayfield->ChangePitchState(MeteoPianoPitchState::Raised);
			//mapPitchShifter->SetSeekPitch(Pitch::c);
			break;
		case MeteoPianoPitchState::Raised:
			break;
		}
	}


	return 0;
}

int OctaveShifter::implementUndoControlPlayfield(EventProcessor<Event>* eProcessor)
{
	LOG(LogLevel::Debug) << "OctaveShifter::implementUndoControlPlayfield() : unshift octave.";
	OctaveShiftEventProcessor* octaveShiftEventProcessor = dynamic_cast<OctaveShiftEventProcessor*>(eProcessor);

	/* 平移八度已經被改掉了
	mapPitchShifter = dynamic_cast<MeteorPlayfield*>(playfield)->GetMapPitchShifter();
	if (mapPitchShifter == nullptr)
		return 0;
	*/

	if (octaveShiftEventProcessor->GetShiftType() == OctaveShiftType::Lower) {
		switch (meteorPlayfield->GetMeteoPianoPitchState()) {
		case MeteoPianoPitchState::Lowered:
			meteorPlayfield->ChangePitchState(MeteoPianoPitchState::None);
			//mapPitchShifter->JumpToPitch(Pitch::C);
			break;
		case MeteoPianoPitchState::None:
			meteorPlayfield->ChangePitchState(MeteoPianoPitchState::Raised);
			//mapPitchShifter->JumpToPitch(Pitch::c);
			break;
		case MeteoPianoPitchState::Raised:
			break;
		}
	}
	else if (octaveShiftEventProcessor->GetShiftType() == OctaveShiftType::Raise) {
		switch (meteorPlayfield->GetMeteoPianoPitchState()) {
		case MeteoPianoPitchState::Lowered:
			break;
		case MeteoPianoPitchState::None:
			meteorPlayfield->ChangePitchState(MeteoPianoPitchState::Lowered);
			//mapPitchShifter->JumpToPitch(Pitch::C1);
			break;
		case MeteoPianoPitchState::Raised:
			meteorPlayfield->ChangePitchState(MeteoPianoPitchState::None);
			//mapPitchShifter->JumpToPitch(Pitch::C);
			break;
		}
	}



	return 0;
}
