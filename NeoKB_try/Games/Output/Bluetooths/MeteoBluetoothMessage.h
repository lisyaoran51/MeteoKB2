#ifndef METEO_BLUETOOTH_MESSAGE_H
#define METEO_BLUETOOTH_MESSAGE_H


#include "../../../Framework/Output/Bluetooths/BluetoothMessage.h"
#include "../../Input/Commands/MeteoCommand.h"


using namespace Framework::Output::Bluetooths;
using namespace Games::Input::Commands;


namespace Games {
namespace Output{
namespace Bluetooths{

	enum class MeteoBluetoothMessageAccessType {
		None,
		ReadOnly,
		WriteOnly,
		ReadWrite
	};

	class MeteoBluetoothMessage : public TBluetoothMessage<MeteoCommand> {


	public:

		MeteoBluetoothMessage(MeteoCommand mCommand);

		virtual string ToString();

		virtual int SetContext(char* c, int cSize) = 0;

		virtual int SetAccessType(MeteoBluetoothMessageAccessType aType) = 0;

	protected:

		MeteoBluetoothMessageAccessType accessType = MeteoBluetoothMessageAccessType::WriteOnly;

	};

}}}





#endif