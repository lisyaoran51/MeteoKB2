#ifndef TRIGGERABLE_H
#define TRIGGERABLE_H



#include <vector>
#include "Updatable.h"
#include "../../Input/InputState.h"

using namespace std;
using namespace Framework::Input;

namespace Framework {
namespace Allocation {
namespace Hierachal {

	/// <summary>
	/// every class inherit form this has a function to update. 每個update都匯被parent呼叫，最後由update thread來叫root
	/// </summary>
	class Triggerable : public Updatable {

	public:

		Triggerable();

		int TriggerOnKeyDown(InputState* inputState, Key key);

		int TriggerOnKeyUp(InputState* inputState, Key key);

		int TriggerOnKnobTurn(InputState* inputState, Knob knob);

		int TriggerOnButtonDown(InputState* inputState, Button button);

		int TriggerOnButtonUp(InputState* inputState, Button button);

		int TriggerOnSlide(InputState* inputState, Slider slider);

	protected:

		virtual int onKeyDown(InputState* inputState, Key key);

		virtual int onKeyUp(InputState* inputState, Key key);

		virtual int onKnobTurn(InputState* inputState, Knob knob);

		virtual int onButtonDown(InputState* inputState, Button button);

		virtual int onButtonUp(InputState* inputState, Button button);

		virtual int onSlide(InputState* inputState, Slider slider);

	};

}}}







#endif