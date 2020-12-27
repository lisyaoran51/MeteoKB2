#ifndef METEO_BLUETOOTH_COMMAND_H
#define METEO_BLUETOOTH_COMMAND_H


#ifndef FIRMWARE_VERSION
#define FIRMWARE_VERSION 0x0
#endif


#include "../../../Framework/Input/Commands/BluetoothCommand.h"
#include "MeteoCommand.h"


using namespace Framework::Input::Commands;


namespace Games {
namespace Input{
namespace Commands{

	class MeteoBluetoothCommand : public TBluetoothCommand<MeteoCommand> {

	public:

		MeteoBluetoothCommand(MeteoCommand c);

		unsigned int firmwareVersion = FIRMWARE_VERSION;

		virtual BluetoothCommand* Clone();

	};

}}}





#endif
