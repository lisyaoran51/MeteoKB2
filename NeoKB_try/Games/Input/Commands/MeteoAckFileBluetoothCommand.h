#ifndef METEO_ACK_FILE_BLUETOOTH_COMMAND_H
#define METEO_ACK_FILE_BLUETOOTH_COMMAND_H


#include "MeteoBluetoothCommand.h"


namespace Games {
namespace Input{
namespace Commands{

	class MeteoAckFileBluetoothCommand : public MeteoBluetoothCommand {

	public:

		MeteoAckFileBluetoothCommand(MeteoCommand c);

		MeteoAckFileBluetoothCommand(MeteoCommand c, int o, string fName);

		int SetOrder(int o);

		int GetOrder();

		int SetFileName(string fName);

		string GetFileName();

	protected:

		int order = -1;

		string fileName = "";

	};

}}}



#endif