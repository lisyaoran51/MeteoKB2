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
	/// 負責去host抓輸入，這個跟keyboard input handler不同的是，他拿到input以後會直接送到instrument去
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