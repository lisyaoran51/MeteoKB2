#ifndef TRIGGERABLE_H
#define TRIGGERABLE_H



#include <vector>
#include "Schedulable.h"
#include "../../Input/InputState.h"

using namespace std;
using namespace Framework::Input;

namespace Framework {
namespace Allocation {
namespace Hierachal {

	/// <summary>
	/// every class inherit form this has a function to update. 每個update都匯被parent呼叫，最後由update thread來叫root
	/// </summary>
	class Triggerable : public Schedulable {

	public:

		Triggerable();

		int TriggerOnKeyDown(InputState* inputState, Key key);

		int TriggerOnKeyUp(InputState* inputState, Key key);

		int TriggerOnKnobTurn(InputState* inputState, PanelKnob knob);

		int TriggerOnButtonDown(InputState* inputState, PanelButton button);

		int TriggerOnButtonUp(InputState* inputState, PanelButton button);

		int TriggerOnSlide(InputState* inputState, PanelSlider slider);

	protected:

		virtual int onKeyDown(InputState* inputState, Key key);

		virtual int onKeyUp(InputState* inputState, Key key);

		virtual int onKnobTurn(InputState* inputState, PanelKnob knob);

		virtual int onButtonDown(InputState* inputState, PanelButton button);

		virtual int onButtonUp(InputState* inputState, PanelButton button);

		virtual int onSlide(InputState* inputState, PanelSlider slider);

	};

}}}







#endif