#ifndef PASS_THROUGH_INPUT_MANAGER_H
#define PASS_THROUGH_INPUT_MANAGER_H


#include "CustomInputManager.h"


namespace Framework {
namespace Input {

	/// <summary>
	/// 他會接到他的parent丟下來的new state，然後當成自己的pending state
	/// </summary>
	class PassThroughInputManager : public CustomInputManager {



		vector<InputState*> pendingParentStates;

		int acceptState(InputState* inputState);

	public:

		PassThroughInputManager();

	protected:

		/// <summary>
		/// 打開狀態時會接收parent丟下來的state，關掉的話就只能靠input handler丟State近來
		/// </summary>
		bool useParentState = true;

		virtual int update();

		virtual vector<InputState*>* getPendingState(vector<InputState*>* pendingStates);

		virtual int onKeyDown(InputState* inputState, InputKey key) override;

		virtual int onKeyUp(InputState* inputState, InputKey key) override;

		virtual int onKnobTurn(InputState* inputState, InputKey knob) override;

		virtual int onButtonDown(InputState* inputState, InputKey button) override;

		virtual int onButtonUp(InputState* inputState, InputKey button) override;

		virtual int onSlide(InputState* inputState, InputKey slider) override;

	};



}}






#endif