#ifndef METEO_TEXT_BLUETOOTH_COMMAND_H
#define METEO_TEXT_BLUETOOTH_COMMAND_H




#include "../../../Framework/Input/Commands/BluetoothCommand.h"
#include "MeteoCommand.h"
#include "MeteoBluetoothCommand.h"


using namespace Framework::Input::Commands;


namespace Games {
namespace Input{
namespace Commands{

	class MeteoTextBluetoothCommand : public MeteoBluetoothCommand {

	public:

		MeteoTextBluetoothCommand(MeteoCommand c);

		virtual BluetoothCommand* Clone();

	};

}}}





#endif
