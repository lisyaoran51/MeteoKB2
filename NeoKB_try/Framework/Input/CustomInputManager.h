#ifndef CUSTOM_INPUT_MANAGER_H
#define CUSTOM_INPUT_MANAGER_H

#include "InputManager.h"



namespace Framework {
namespace Input {

	class CustomInputManager : public InputManager {

		vector<InputHandler*> inputHandlers;

	public:

		CustomInputManager();

	protected:

		/// <summary>
		/// �bget pending state�ɥX�Ӵ���input handler�A�ӧ�input state
		/// �o�����input handler�]���Ŧ�C�A�M��A�ۭqinput handler
		/// </summary>
		virtual vector<InputHandler*>* getInputHandlers();

		int AddHandler(InputHandler* inputHandler);

		int RemoveHandler(InputHandler* inputHandler);
	};


}}




#endif