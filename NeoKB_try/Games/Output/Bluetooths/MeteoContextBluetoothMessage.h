#ifndef METEO_CONTEXT_BLUETOOTH_MESSAGE_H
#define METEO_CONTEXT_BLUETOOTH_MESSAGE_H


#include "MeteoBluetoothMessage.h"




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