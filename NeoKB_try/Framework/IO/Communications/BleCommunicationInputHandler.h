#ifndef BLE_COMMUNICATION_INPUT_HANDLER_H
#define BLE_COMMUNICATION_INPUT_HANDLER_H



#include <vector>
#include "../../Input/Handler/InputHandler.h"
#include "../../Hosts/Host.h"


using namespace Framework::Hosts;
using namespace std;
using namespace Framework::Input::Handler;


namespace Framework {
namespace IO {
namespace Communications {

	class CommunicationAccess;

	/// <summary>
	/// �t�d�hhost���J�A�o�Ӹ�keyboard input handler���P���O�A�L����input�H��|�����e��instrument�h
	/// </summary>
	class BleCommunicationInputHandler : public InputHandler {

	public:

		BleCommunicationInputHandler(CommunicationAccess* cAccess);

		virtual int Initialize(Host* host);

		virtual int HandleState(InputState* inputEvent);

	protected:
		
		CommunicationAccess* communicationAccess = nullptr;

	private:

		

	};


}}}











#endif