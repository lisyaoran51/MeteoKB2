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

		int updateKeyboardEvents(InputState* inputState);

		int updatePanelEvents(InputState* inputState);

		int updateBluetoothEvents(InputState* inputState);

		int handleKeyDown(InputState* state, Key key);

		int propagateKeyDown(InputState* state, Key key);

		int handleKeyUp(InputState* state, Key key);

		int propagateKeyUp(InputState* state, Key key);

		int handleKnobTurn(InputState* state, Knob knob);

		int propagateKnobTurn(InputState* state, Knob knob);

	private:

		/// <summary>
		/// 擺入gamehost
		/// </summary>
		int load();

		vector<InputHandler*> inputHandlers;

		int iterateGetDeepestChild(Triggerable* deepestChild, int* deepestDepth, int tempDepth);



	};


}}

#endif