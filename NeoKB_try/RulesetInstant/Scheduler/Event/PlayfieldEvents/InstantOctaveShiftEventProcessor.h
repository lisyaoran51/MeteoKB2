#ifndef INSTANT_OCTAVE_SHIFT_EVENT_PROCESSOR_H
#define INSTANT_OCTAVE_SHIFT_EVENT_PROCESSOR_H

#include "../../../../Games/Scheduler/Event/PlayfieldEvents/PlayfieldEventProcessor.h"
#include "InstantOctaveShiftEvent.h"


using namespace Games::Schedulers::Events::PlayfieldEvents;



namespace Instant {
namespace Schedulers{
namespace Events{
namespace PlayfieldEvents{

	class InstantOctaveShiftEventProcessor : public PlayfieldEventProcessor<InstantOctaveShiftEvent> {

	public:

		InstantOctaveShiftEventProcessor();

		InstantOctaveShiftType GetShiftType();

		float GetShiftStartTime();

	};


}}}}


#endif