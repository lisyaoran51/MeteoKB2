#include "OctaveShifter.h"

#include "../../../../Play/MeteorPlayfield.h"


using namespace Meteor::Schedulers::Events::PlayfieldEvents::PlayfieldControllers;
using namespace Meteor::Play;



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

	mapPitchShifter = dynamic_cast<MeteorPlayfield*>(playfield)->GetMapPitchShifter();
	if (mapPitchShifter == nullptr)
		return 0;


	OctaveShiftEventProcessor* octaveShiftEventProcessor = dynamic_cast<OctaveShiftEventProcessor*>(eProcessor);

	double framesPerSecond = GetClock()->GetFramesPerSecond();

	float shiftTime = octaveShiftEventProcessor->GetShiftTimeLength();

	if (shiftTime == -1) {
		mapPitchShifter->SetSeekSpeed(framesPerSecond);
	}
	else if (shiftTime == 0) {
		mapPitchShifter->SetFloatSeekSpeed(12.0, 1.0);
	}
	else
		mapPitchShifter->SetFloatSeekSpeed(12.0, shiftTime);

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
			break;
		case MeteoPianoPitchState::Raised:
			meteorPlayfield->ChangePitchState(MeteoPianoPitchState::None);
			break;
		}
	}
	else if(octaveShiftEventProcessor->GetShiftType() == OctaveShiftType::Raise) {
		switch (meteorPlayfield->GetMeteoPianoPitchState()) {
		case MeteoPianoPitchState::Lowered:
			meteorPlayfield->ChangePitchState(MeteoPianoPitchState::None);
			break;
		case MeteoPianoPitchState::None:
			meteorPlayfield->ChangePitchState(MeteoPianoPitchState::Raised);
			break;
		case MeteoPianoPitchState::Raised:
			break;
		}
	}


	return 0;
}
