#include "InstantOctaveShifter.h"

#include "../../../../Scenes/Play/InstantPlayfield.h"
#include "../../../../../Util/Log.h"


using namespace Instant::Schedulers::Events::PlayfieldEvents::PlayfieldControllers;
using namespace Instant::Scenes::Play;
using namespace Util;


InstantOctaveShifter::InstantOctaveShifter(): RegisterType("InstantOctaveShifter")
{
}

int InstantOctaveShifter::LazyConstruct(Playfield * p)
{
	PlayfieldController::LazyConstruct(p);

	instantPlayfield = dynamic_cast<InstantPlayfield*>(playfield);

	return 0;
}

int InstantOctaveShifter::implementControlPlayfield(EventProcessor<Event>* eProcessor)
{
	LOG(LogLevel::Debug) << "OctaveShifter::implementControlPlayfield() : shift octave.";
	InstantOctaveShiftEventProcessor* octaveShiftEventProcessor = dynamic_cast<InstantOctaveShiftEventProcessor*>(eProcessor);



	if (octaveShiftEventProcessor->GetShiftType() == InstantOctaveShiftType::Lower) {
		switch (instantPlayfield->GetMeteoPianoPitchState()) {
		case MeteoPianoPitchState::Lowered:
			break;
		case MeteoPianoPitchState::None:
			instantPlayfield->ChangePitchState(MeteoPianoPitchState::Lowered);
			//mapPitchShifter->SetSeekPitch(Pitch::C1);
			break;
		case MeteoPianoPitchState::Raised:
			instantPlayfield->ChangePitchState(MeteoPianoPitchState::None);
			//mapPitchShifter->SetSeekPitch(Pitch::C);
			break;
		}
	}
	else if(octaveShiftEventProcessor->GetShiftType() == InstantOctaveShiftType::Raise) {
		switch (instantPlayfield->GetMeteoPianoPitchState()) {
		case MeteoPianoPitchState::Lowered:
			instantPlayfield->ChangePitchState(MeteoPianoPitchState::None);
			//mapPitchShifter->SetSeekPitch(Pitch::C);
			break;
		case MeteoPianoPitchState::None:
			instantPlayfield->ChangePitchState(MeteoPianoPitchState::Raised);
			//mapPitchShifter->SetSeekPitch(Pitch::c);
			break;
		case MeteoPianoPitchState::Raised:
			break;
		}
	}


	return 0;
}

int InstantOctaveShifter::implementUndoControlPlayfield(EventProcessor<Event>* eProcessor)
{

	return 0;
}
