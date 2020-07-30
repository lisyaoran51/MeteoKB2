#ifndef METEO_BLUETOOTH_MESSAGE_H
#define METEO_BLUETOOTH_MESSAGE_H


#include "../../../Framework/Output/Bluetooths/BluetoothMessage.h"
#include "../../Input/Commands/MeteoBluetoothCommand.h"


using namespace Framework::Output::Bluetooths;
using namespace Games::Input::Commands;


namespace Games {
namespace Output{
namespace Bluetooths{

	class MeteoBluetoothMessage : public BluetoothMessage {


	public:

		MeteoBluetoothMessage(MeteoCommand mCommand);

		json& GetContext();

	protected:

		MeteoCommand meteoCommand;

		json context;

	};

}}}





#endif