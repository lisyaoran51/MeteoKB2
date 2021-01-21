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
	/// ���input�A�åB�o�e��Ҧ��l����
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
		/// �bget pending state�ɥX�Ӵ���input handler�A�ӧ�input state
		/// </summary>
		virtual vector<InputHandler*>* getInputHandlers() = 0;


		InputState* currentState = nullptr;

		/// <summary>
		/// �p�G��state�A�|��update input queue�A�M��A��̩��ݪ�child��trigger
		/// </summary>
		virtual int handleNewState(InputState* state);

		virtual vector<InputState*>* getPendingState(vector<InputState*>* pendingStates);

		virtual int updateInputQueue(InputState* inputState);

		/// <summary>
		/// �|��Ҧ���J���\��ӡA�M��ƻs�@�ӷs��input state�A�p�Gkb,panel,bt�Onull���e�|new�@�ӪŪ��\�i�h
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
		/// �\�Jgamehost
		/// </summary>
		int load();

		int load(GameHost* h);

		//vector<InputHandler*> inputHandlers;

		Triggerable* focusTriggerable = nullptr;

		int iterateUpdateInputQueue(Triggerable* p, vector<Triggerable*>* tQueue);



	};


}}

#endif