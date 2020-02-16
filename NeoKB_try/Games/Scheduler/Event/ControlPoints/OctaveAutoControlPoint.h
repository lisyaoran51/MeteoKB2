#ifndef OCTAVE_AUTO_CONTROL_POINT_H
#define OCTAVE_AUTO_CONTROL_POINT_H


#include "AutoControlPoint.h"
#include "../../../../Framework/Input/KeyBindings/HasInputKey.h"



using namespace Framework::Input::KeyBindings;



namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{

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
