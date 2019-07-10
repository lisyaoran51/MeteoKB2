#ifndef BLUETOOTH_INPUT_HANDLER_H
#define BLUETOOTH_INPUT_HANDLER_H


#include <vector>
#include "InputHandler.h"

using namespace std;


namespace Framework {
namespace Input {
namespace Handler {

	/// <summary>
	/// ­t³d¥hhost§ì¿é¤J
	/// </summary>
	class BluetoothInputHandler : public InputHandler {

	public:

		virtual int Initialize(GameHost* host);

		virtual int HandleState(InputState* inputState);

	protected:
		

	private:

		

	};


}}}

#endif