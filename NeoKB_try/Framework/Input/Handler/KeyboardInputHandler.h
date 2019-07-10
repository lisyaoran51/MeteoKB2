#ifndef KEYBOARD_INPUT_HANDLER_H
#define KEYBOARD_INPUT_HANDLER_H


#include <vector>
#include "InputHandler.h"

using namespace std;


namespace Framework {
namespace Input {
namespace Handler {

	/// <summary>
	/// ­t³d¥hhost§ì¿é¤J
	/// </summary>
	class KeyboardInputHandler : public InputHandler {

	public:

		virtual int Initialize(GameHost* host);

		virtual int HandleState(InputState* inputEvent);

	protected:
		

	private:

		

	};


}}}

#endif