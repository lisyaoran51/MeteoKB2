#ifndef INSTANT_OCTAVE_SHIFT_EVENT_H
#define INSTANT_OCTAVE_SHIFT_EVENT_H

#include "../../../../Games/Scheduler/Event/PlayfieldEvents/PlayfieldEvent.h"


using namespace Games::Schedulers::Events::PlayfieldEvents;


namespace Instant {
namespace Schedulers{
namespace Events{
namespace PlayfieldEvents{

	enum class InstantOctaveShiftType {
		None,
		Lower,
		Raise
	};

	class InstantOctaveShiftEvent : public PlayfieldEvent {

	public:

		InstantOctaveShiftEvent(InstantOctaveShiftType oShiftType, MTO_FLOAT s, MTO_FLOAT l);

		// 一定要每次都override!!
		virtual string GetTypeName();

		virtual Event* Clone();

		InstantOctaveShiftType GetShiftType();

		float GetShiftStartTime();

	protected:

		InstantOctaveShiftType octaveShiftType = InstantOctaveShiftType::None;

	};


}}}}


#endif