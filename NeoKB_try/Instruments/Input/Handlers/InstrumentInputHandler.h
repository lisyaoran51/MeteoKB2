#ifndef INSTRUMENT_INPUT_HANDLER
#define INSTRUMENT_INPUT_HANDLER


#include <vector>
#include "../../../Framework/Input/Handler/InputHandler.h"
#include "../../../Framework/Hosts/Host.h"
#include "../../../Framework/Input/KeyBindings/KeyBinding.h"


using namespace Framework::Hosts;
using namespace std;
using namespace Framework::Input::Handler;
using namespace Framework::Input::KeyBindings;


namespace Instruments {
	class Piano;
}


namespace Instruments {
namespace Input {
namespace Handlers {

	/// <summary>
	/// 負責去host抓輸入，這個跟keyboard input handler不同的是，他拿到input以後會直接送到instrument去
	/// </summary>
	class InstrumentInputHandler : public InputHandler {

	public:

		InstrumentInputHandler(Piano* p);

		virtual int Initialize(Host* host);

		virtual int HandleState(InputState* inputEvent);

	protected:
		
		Piano* piano = nullptr;

		vector<KeyBinding*>* keyBindings = nullptr;

	private:

		

	};


}}}









#endif