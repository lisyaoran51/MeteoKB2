#ifndef MARK_CONTROL_POINT_H
#define MARK_CONTROL_POINT_H


#include "ControlPoint.h"



namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{


	/// <summary>
	/// 單純標記當下資訊的control point
	///	</summary>
	class MarkControlPoint : public ControlPoint {

	public:

		MarkControlPoint(MTO_FLOAT s, MTO_FLOAT l);

		int GetSectionIndex();
		int SetSectionIndex(int sI);

		int GetPartIndex();
		int SetPartIndex(int pIndex);

		// 一定要每次都override!!
		virtual string GetTypeName();

		virtual ControlPointType GetControlPointType();

	protected:

		int sectionIndex = -1;
		int partIndex = -1;

	};





}}}}






#endif