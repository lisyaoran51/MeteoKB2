#ifndef OCTAVE_AUTO_CONTROL_POINT_H
#define OCTAVE_AUTO_CONTROL_POINT_H


#include "AutoControlPoint.h"
#include "../../../../Framework/Input/KeyBindings/HasInputKey.h"



using namespace Framework::Input::KeyBindings;



namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{

	/// <summary>
	/// 平移八度的特效，結果沒有用到
	/// 這邊的life time是指平移八度移過去花多少時間，不是指維持這個音域的時長多久
	///	</summary>
	class OctaveAutoControlPoint : public AutoControlPoint, public HasInputKey {


	public:

		OctaveAutoControlPoint(InputKey iKey, MTO_FLOAT s, MTO_FLOAT l);

		OctaveAutoControlPoint(InputKey iKey, int iValue, MTO_FLOAT s, MTO_FLOAT l);

		virtual ControlPointType GetControlPointType();

		// 一定要每次都override!!
		virtual string GetTypeName();

		virtual Event* Clone();

	};

}}}}



#endif
