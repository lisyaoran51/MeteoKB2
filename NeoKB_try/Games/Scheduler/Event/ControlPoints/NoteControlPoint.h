#ifndef NOTE_CONTROL_POINT_H
#define NOTE_CONTROL_POINT_H

#include "PlayableControlPoint.h"
#include "../../../../Instruments/Pitch.h"
#include "../../../../Instruments/HasPitch.h"


using namespace Instruments;
using namespace Games::Schedulers::Events;


namespace Games {
namespace Schedulers {
namespace Events {
namespace ControlPoints {

	class NoteControlPoint : public PlayableControlPoint, public HasPitch {


	public:

		NoteControlPoint(Pitch p, MTO_FLOAT s, MTO_FLOAT l);

		virtual ControlPointType GetControlPointType();

		// 一定要每次都override!!
		virtual string GetTypeName();

		bool IsWhiteKey();

		virtual Event* Clone();

	};

	
	
	
}}}}





#endif