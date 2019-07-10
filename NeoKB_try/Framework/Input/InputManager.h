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
	/// ���input�A�åB�o�e��Ҧ��l����
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
		/// �p�G��state�A�|��update input queue�A�M��A��̩��ݪ�child��trigger
		/// </summary>
		int handleNewState(InputState* state);

		vector<InputState*>* getPendingState(vector<InputState*>* pendingStates);

		virtual int updateInputQueue(InputState* inputState);

		/// <summary>
		/// ���ӽT�w�\��A�Ȯɥ�����
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
		/// �\�Jgamehost
		/// </summary>
		int load();

		vector<InputHandler*> inputHandlers;

		int iterateGetDeepestChild(Triggerable* deepestChild, int* deepestDepth, int tempDepth);



	};


}}

#endif