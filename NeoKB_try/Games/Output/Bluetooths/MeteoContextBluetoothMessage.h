#ifndef METEO_CONTEXT_BLUETOOTH_MESSAGE_H
#define METEO_CONTEXT_BLUETOOTH_MESSAGE_H


#include "../../../Framework/Output/Bluetooths/BluetoothMessage.h"
#include "../../Input/Commands/MeteoBluetoothCommand.h"
#include "MeteoBluetoothMessage.h"


using namespace Framework::Output::Bluetooths;
using namespace Games::Input::Commands;


namespace Games {
namespace Output{
namespace Bluetooths{

	class MeteoContextBluetoothMessage : public MeteoBluetoothMessage {


	public:

		MeteoContextBluetoothMessage(MeteoCommand mCommand);

		json& GetContext();

	protected:

		json context;

	};

}}}





#endif