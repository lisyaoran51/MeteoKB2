#ifndef OCTAVE_SHIFT_EVENT_H
#define OCTAVE_SHIFT_EVENT_H

#include "../../../../Games/Scheduler/Event/PlayfieldEvents/PlayfieldEvent.h"


using namespace Games::Schedulers::Events::PlayfieldEvents;


namespace Meteor {
namespace Schedulers{
namespace Events{
namespace PlayfieldEvents{

	enum class OctaveShiftType {
		None,
		Lower,
		Raise
	};

	class OctaveShiftEvent : public PlayfieldEvent {

	public:

		OctaveShiftEvent(OctaveShiftType oShiftType, MTO_FLOAT s, MTO_FLOAT l);

		// 一定要每次都override!!
		virtual string GetTypeName();

		virtual Event* Clone();

		OctaveShiftType GetShiftType();

		float GetShiftStartTime();

		float GetShiftTimeLength();

	protected:

		OctaveShiftType octaveShiftType = OctaveShiftType::None;

	};


}}}}


#endif