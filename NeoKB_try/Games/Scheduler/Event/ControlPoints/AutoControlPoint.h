#ifndef AUTO_CONTROL_POINT_H
#define AUTO_CONTROL_POINT_H


#include "HandType.h"
#include "MarkControlPoint.h"



namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{


	/// <summary>
	/// 不須輸入就會自動執行的control point
	///	</summary>
	class AutoControlPoint : public MarkControlPoint {

	public:

		AutoControlPoint(MTO_FLOAT s, MTO_FLOAT l);

		MTO_FLOAT GetVolume();
		int SetVolume(MTO_FLOAT v);

		HandType GetHandType();
		int SetHandType(HandType h);

		// 一定要每次都override!!
		virtual string GetTypeName();


	protected:

		MTO_FLOAT volume = -1;

		HandType handType = HandType::Auto;

	};





}}}}






#endif