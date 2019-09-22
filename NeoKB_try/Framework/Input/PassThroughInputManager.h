#ifndef PASS_THROUGH_INPUT_MANAGER_H
#define PASS_THROUGH_INPUT_MANAGER_H


#include "InputManager.h"


namespace Framework {
namespace Input {

	/// <summary>
	/// �L�|����L��parent��U�Ӫ�new state�A�M����ۤv��pending state
	/// </summary>
	class PassThroughInputManager : public InputManager {


		/// <summary>
		/// ���}���A�ɷ|����parent��U�Ӫ�state�A�������ܴN�u��ainput handler��State���
		/// </summary>
		bool useParentState = true;

		vector<InputState*> pendingParentStates;

		int acceptState(InputState* inputState);

	public:



	protected:

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