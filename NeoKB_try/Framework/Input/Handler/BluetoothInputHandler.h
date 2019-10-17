#ifndef BLUETOOTH_INPUT_HANDLER_H
#define BLUETOOTH_INPUT_HANDLER_H


#include <vector>
#include "InputHandler.h"
#include "../../Host/GameHost.h"



using namespace std;
using namespace Framework::Host;



namespace Framework {
namespace Input {
namespace Handler {

	/// <summary>
	/// �t�d�hhost���J
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