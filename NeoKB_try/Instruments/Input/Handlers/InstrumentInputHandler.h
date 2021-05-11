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
	/// �t�d�hhost���J�A�o�Ӹ�keyboard input handler���P���O�A�L����input�H��|�����e��instrument�h
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