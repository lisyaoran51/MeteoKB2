#ifndef SECTION_START_CONTROL_POINT_H
#define SECTION_START_CONTROL_POINT_H


#include "MarkControlPoint.h"



namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{


	/// <summary>
	/// 標記小節起始點的control point
	///	</summary>
	class SectionStartControlPoint : public MarkControlPoint {

	public:

		SectionStartControlPoint(int sIndex, MTO_FLOAT s, MTO_FLOAT l);

		// 一定要每次都override!!
		virtual string GetTypeName();

		virtual Event* Clone();

	protected:

	};





}}}}






#endif