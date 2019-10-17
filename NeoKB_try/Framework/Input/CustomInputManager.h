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
		/// 在get pending state時出來提供input handler，來抓input state
		/// 這邊先把input handler設成空串列，然後再自訂input handler
		/// </summary>
		virtual vector<InputHandler*>* getInputHandlers();

		int AddHandler(InputHandler* inputHandler);

		int RemoveHandler(InputHandler* inputHandler);
	};


}}




#endif