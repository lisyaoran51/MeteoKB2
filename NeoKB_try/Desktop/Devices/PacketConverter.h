#ifndef PACKET_CONVERTER_H
#define PACKET_CONVERTER_H



#include "../../Framework/Input/Commands/BluetoothCommand.h"
#include <map>
#include "../../Framework/Output/Bluetooths/BluetoothMessage.h"


using namespace Framework::Input::Commands;
using namespace std;
using namespace Framework::Output::Bluetooths;


namespace Desktop {
namespace Devices{

	enum class PacketType {
		None,
		Json,
		File,
		AckFile,
	};


	template<typename T>
	class PacketConverter {

	public:

		PacketConverter<T>* SetJsonPacketTypeCommand(T command) {
			CommandPacketTypeMap[command] = PacketType::Json;
			return this;
		}

		PacketConverter<T>* SetFilePacketTypeCommand(T command) {
			CommandPacketTypeMap[command] = PacketType::File;
			return this;
		}

		PacketConverter<T>* SetAckFilePacketTypeCommand(T command) {
			CommandPacketTypeMap[command] = PacketType::AckFile;
			return this;
		}

		virtual PacketType CheckPacketType(char* buffer, int size) = 0;

		virtual BluetoothCommand* ConvertToBluetoothCommand(char* buffer, int size) = 0;

		virtual BluetoothCommand* ConvertToBluetoothCommand(BluetoothMessage* bluetoothMessage) = 0;

		virtual int ConvertToByteArray(BluetoothCommand* bluetoothCommand, char* buffer, int bufferMaxSize) = 0;

		virtual string ConvertToFile(char* buffer, int size) = 0;

	protected:

		map<T, PacketType> CommandPacketTypeMap;

	};

}
}





#endif