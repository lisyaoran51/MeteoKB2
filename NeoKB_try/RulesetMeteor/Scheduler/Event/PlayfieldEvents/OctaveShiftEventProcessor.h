#ifndef OCTAVE_SHIFT_EVENT_PROCESSOR_H
#define OCTAVE_SHIFT_EVENT_PROCESSOR_H

#include "../../../../Games/Scheduler/Event/PlayfieldEvents/PlayfieldEventProcessor.h"
#include "OctaveShiftEvent.h"


using namespace Games::Schedulers::Events::PlayfieldEvents;



namespace Meteor {
namespace Schedulers{
namespace Events{
namespace PlayfieldEvents{

	class OctaveShiftEventProcessor : public PlayfieldEventProcessor<OctaveShiftEvent> {

	public:

		OctaveShiftEventProcessor();

		OctaveShiftType GetShiftType();

		float GetShiftStartTime();

		float GetShiftTimeLength();

	};


}}}}


#endif