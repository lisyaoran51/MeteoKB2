#ifndef METEO_BLUETOOTH_OUTPUTER_H
#define METEO_BLUETOOTH_OUTPUTER_H


#include "../../../Framework/Output/Bluetooths/BluetoothOutputer.h"
#include "MeteoBluetoothMessage.h"


using namespace Framework::Output::Bluetooths;


namespace Games {
namespace Output{
namespace Bluetooths{

	class MeteoBluetoothOutputer : public TBluetoothOutputer<MeteoBluetoothMessage> {

	protected:

		virtual int pushMessage(MeteoBluetoothMessage* outputMessage);

	};

}}}







#endif