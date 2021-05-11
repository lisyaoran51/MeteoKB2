#ifndef KEYBOARD_INPUT_HANDLER_H
#define KEYBOARD_INPUT_HANDLER_H


#include <vector>
#include "InputHandler.h"
#include "../../Hosts/Host.h"

using namespace std;
using namespace Framework::Hosts;


namespace Framework {
namespace Input {
namespace Handler {

	/// <summary>
	/// ­t³d¥hhost§ì¿é¤J
	/// </summary>
	class KeyboardInputHandler : public InputHandler {

	public:

		virtual int Initialize(Host* host);

		virtual int HandleState(InputState* inputEvent);

	protected:
		

	private:

		

	};


}}}

#endif