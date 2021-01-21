#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H


#include <vector>

#include "../../Framework/Allocation/Hierachal/Container.h"
#include "Handler/InputHandler.h"
#include "KeyBindings/InputKey.h"


using namespace std;
using namespace Framework::Allocation::Hierachal;
using namespace Framework::Input::Handler;
using namespace Framework::Input::KeyBindings;

namespace Framework {
namespace Input {

	/// <summary>
	/// 抓取input，並且發送到所有子物件
	/// </summary>
	class InputManager : public Container {

	public:

		InputManager();

		int ChangeFocus(Triggerable* fTriggerable);

		//int AddStaticTriggerable(Triggerable* sTriggerable);

		//int DeleteStaticTriggerable(Triggerable* sTriggerable);

		//int ClearStaticTriggerable();

	protected:

		GameHost* host;

		vector<Triggerable*> triggerQueue;

		vector<Triggerable*> staticTriggerQueue;

		vector<InputState*> pendingStates;

		virtual int update();

		/// <summary>
		/// 在get pending state時出來提供input handler，來抓input state
		/// </summary>
		virtual vector<InputHandler*>* getInputHandlers() = 0;


		InputState* currentState = nullptr;

		/// <summary>
		/// 如果有state，會先update input queue，然後再找最底端的child來trigger
		/// </summary>
		virtual int handleNewState(InputState* state);

		virtual vector<InputState*>* getPendingState(vector<InputState*>* pendingStates);

		virtual int updateInputQueue(InputState* inputState);

		/// <summary>
		/// 會把所有輸入都擺近來，然後複製一個新的input state，如果kb,panel,bt是null的畫會new一個空的擺進去
		/// </summary>
		vector<InputState*>* createDistinctInputStates(vector<InputState*>* states);

		virtual int TransformState(InputState* inputState);

		virtual int updateKeyboardEvents(InputState* inputState);

		virtual int updatePanelEvents(InputState* inputState);

		virtual int updateBluetoothEvents(InputState* inputState);

		virtual int handleKeyDown(InputState* state, InputKey key);

		virtual int propagateKeyDown(vector<Triggerable*>* queue, InputState* state, InputKey key);

		virtual int handleKeyUp(InputState* state, InputKey key);

		virtual int propagateKeyUp(vector<Triggerable*>* queue, InputState* state, InputKey key);

		virtual int handleButtonDown(InputState* state, InputKey button);

		virtual int propagateButtonDown(vector<Triggerable*>* queue, InputState* state, InputKey button);

		virtual int handleButtonUp(InputState* state, InputKey button);

		virtual int propagateButtonUp(vector<Triggerable*>* queue, InputState* state, InputKey button);

		virtual int handleKnobTurn(InputState* state, InputKey knob);

		virtual int propagateKnobTurn(vector<Triggerable*>* queue, InputState* state, InputKey knob);

		virtual int handleSlide(InputState* state, InputKey slider);

		virtual int propagateSlide(vector<Triggerable*>* queue, InputState* state, InputKey slider);

		virtual int handleBluetoothMessage(InputState* state, BluetoothMessage* message);

		virtual int propagateBluetoothMessage(vector<Triggerable*>* queue, InputState* state, BluetoothMessage* message);

	private:

		/// <summary>
		/// 擺入gamehost
		/// </summary>
		int load();

		int load(GameHost* h);

		//vector<InputHandler*> inputHandlers;

		Triggerable* focusTriggerable = nullptr;

		int iterateUpdateInputQueue(Triggerable* p, vector<Triggerable*>* tQueue);



	};


}}

#endif