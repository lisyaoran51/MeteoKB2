#ifndef INPUT_KEY_CONTROL_POINT_H
#define INPUT_KEY_CONTROL_POINT_H


#include "PlayableControlPoint.h"
#include "../../../../Framework/Input/KeyBindings/HasInputKey.h"



using namespace Framework::Input::KeyBindings;



namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{

	class InputKeyControlPoint : public PlayableControlPoint, public HasInputKey {


	public:

		InputKeyControlPoint(InputKey iKey, MTO_FLOAT s, MTO_FLOAT l);

		InputKeyControlPoint(InputKey iKey, int iValue, MTO_FLOAT s, MTO_FLOAT l);

		virtual ControlPointType GetControlPointType();

		// 一定要每次都override!!
		virtual string GetTypeName();

		virtual Event* Clone();



	};

}}}}



#endif
