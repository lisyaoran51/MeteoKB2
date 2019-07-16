#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H


#include <vector>

#include "../../Framework/Allocation/Hierachal/Triggerable.h"
#include "Handler\InputHandler.h"


using namespace std;
using namespace Framework::Allocation::Hierachal;
using namespace Framework::Input::Handler;

namespace Framework {
namespace Input {

	/// <summary>
	/// 抓取input，並且發送到所有子物件
	/// </summary>
	class InputManager : public Triggerable {

	public:

		InputManager();

		int ChangeFocus(Triggerable* fTriggerable);

		int AddStaticTriggerable(Triggerable* sTriggerable);

		int DeleteStaticTriggerable(Triggerable* sTriggerable);

		int ClearStaticTriggerable();

		Triggerable* GetDeepestChild();

	protected:

		GameHost* host;

		vector<Triggerable*> triggerQueue;

		vector<Triggerable*> staticTriggerQueue;

		virtual int update();


		InputState* currentState;

		/// <summary>
		/// 如果有state，會先update input queue，然後再找最底端的child來trigger
		/// </summary>
		int handleNewState(InputState* state);

		vector<InputState*>* getPendingState(vector<InputState*>* pendingStates);

		virtual int updateInputQueue(InputState* inputState);

		/// <summary>
		/// 不太確定功能，暫時先不用
		/// </summary>
		vector<InputState*>* createDistinctInputStates(vector<InputState*>* states);

		int TransformState(InputState* inputState);

		virtual int updateKeyboardEvents(InputState* inputState);

		virtual int updatePanelEvents(InputState* inputState);

		virtual int updateBluetoothEvents(InputState* inputState);

		virtual int handleKeyDown(InputState* state, Key key);

		virtual int propagateKeyDown(vector<Triggerable*>* queue, InputState* state, Key key);

		virtual int handleKeyUp(InputState* state, Key key);

		virtual int propagateKeyUp(vector<Triggerable*>* queue, InputState* state, Key key);

		virtual int handleButtonDown(InputState* state, Button button);

		virtual int propagateButtonDown(vector<Triggerable*>* queue, InputState* state, Button button);

		virtual int handleButtonUp(InputState* state, Button button);

		virtual int propagateButtonUp(vector<Triggerable*>* queue, InputState* state, Button button);

		virtual int handleKnobTurn(InputState* state, Knob knob);

		virtual int propagateKnobTurn(vector<Triggerable*>* queue, InputState* state, Knob knob);

		virtual int handleSlide(InputState* state, Slider slider);

		virtual int propagateSlide(vector<Triggerable*>* queue, InputState* state, Slider slider);

	private:

		/// <summary>
		/// 擺入gamehost
		/// </summary>
		int load();

		vector<InputHandler*> inputHandlers;

		Triggerable* iterateGetDeepestChild(Triggerable* deepestChild, int* deepestDepth, int tempDepth);



	};


}}

#endif