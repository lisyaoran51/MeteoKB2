#ifndef OCTAVE_SHIFT_SPAN_CONTROL_POINT_H
#define OCTAVE_SHIFT_SPAN_CONTROL_POINT_H


#include "MarkControlPoint.h"



namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{


	/// <summary>
	/// 在哪個音域，這邊不允許處在None的音域，一定要是lowered或raised才有必要建立成一個event
	///	</summary>
	enum class OctaveRange {
		None,		
		Lowered,
		Raised
	};


	/// <summary>
	/// 標記調整八度音域的起始點與維持多久，這邊的life time是指在這個音域維持了多久
	///	</summary>
	class OctaveShiftSpanControlPoint : public MarkControlPoint {

	public:

		OctaveShiftSpanControlPoint(OctaveRange oRange, MTO_FLOAT s, MTO_FLOAT l);

		int SetOctaveRange(OctaveRange oRange);
		OctaveRange GetOctaveRange();

		// 一定要每次都override!!
		virtual string GetTypeName();

	protected:

		OctaveRange octaveRange = OctaveRange::None;

	};





}}}}






#endif