#ifndef BLUETOOTH_PANEL_H
#define BLUETOOTH_PANEL_H


#include "../../Framework/Allocation/Hierachal/Container.h"
#include "../../Framework/Output/OutputManager.h"
#include "../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Framework/Input/Messages/MessageHandler.h"


using namespace Framework::Output;
using namespace Games::Output::Bluetooths;
using namespace Framework::Input::Messages;


namespace Games {
namespace UI{

	class BluetoothPanel : public Container, public MessageHandler<MeteoBluetoothMessage> {

		int load();

		int load(OutputManager* o);

	public:

		BluetoothPanel();

		function<int(void)> BluetoothOffRequest;

		function<int(void)> BluetoothOnRequest;

	protected:

		OutputManager* outputManager = nullptr;

		bool isBluetoothOn = false;

		virtual int onMessage(MeteoBluetoothMessage* message);
		


	};

}}





#endif