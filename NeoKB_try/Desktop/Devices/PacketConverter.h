#ifndef PACKET_CONVERTER_H
#define PACKET_CONVERTER_H



#include <map>
#include "../../Framework/Output/Bluetooths/BluetoothMessage.h"


using namespace std;
using namespace Framework::Output::Bluetooths;


namespace Desktop {
namespace Devices{

	enum class PacketType {
		None,
		ReadFirmwareVersion,
		ReturnFirmwareVersion,
		Json,
		File,
		AckFile,
	};

	enum class PacketStatus {
		None,
		Fine,
		Overlength,		// 封包長度大於length
		Underlength,	// 封包長度小於length
		OutOfBound,		// length超過最大封包大小(538)
		Damaged,		// 封包損壞
		WrongCommand,	// 沒有這個command
		WrongLength,	// 沒有這個封包長度 ex:0
		WrongJsonFormat,// json格式錯誤
	};


	template<typename T>
	class PacketConverter {

	public:

		PacketConverter<T>* SetReadFirmwareVersionPacketTypeCommand(T command) {
			CommandPacketTypeMap[command] = PacketType::ReadFirmwareVersion;
			return this;
		}

		PacketConverter<T>* SetReturnFirmwareVersionPacketTypeCommand(T command) {
			CommandPacketTypeMap[command] = PacketType::ReturnFirmwareVersion;
			return this;
		}

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

		virtual int SplitPacket(const char* bufferIn, int bytesRead, char** packets, int* packerLengths) = 0;

		virtual PacketStatus CheckPacketStatus(const char* packet, int length) = 0;

		virtual PacketType CheckPacketType(const char* buffer, int size) = 0;

		virtual PacketType CheckCommandType(BluetoothMessage* bluetoothMessage) = 0;

		virtual BluetoothMessage* ConvertToBluetoothMessage(const char* buffer, int size) = 0;

		//virtual BluetoothCommand* ConvertToBluetoothCommand(BluetoothMessage* bluetoothMessage) = 0;

		virtual int GetCountOfPacket(BluetoothMessage* bluetoothMessage) = 0;

		virtual int ConvertToByteArray(BluetoothMessage* bluetoothMessage, int packetOrder, char* buffer, int bufferMaxSize) = 0;

		/// <summary>
		/// 回傳array長度
		/// </summary>
		virtual int ConvertToByteArray(BluetoothMessage* bluetoothMessage, char* buffer, int bufferMaxSize) = 0;

		virtual BluetoothMessage* ConvertToFile(const char* buffer, int size) = 0;

		//virtual bool CheckIsFinishWriteCommand(BluetoothMessage* bluetoothMessage) = 0;

		//virtual BluetoothMessage* FinishWriteFile(BluetoothMessage* bluetoothMessage) = 0;

		//virtual bool CheckIsWrtieFileFinishCommand(BluetoothMessage* bluetoothMessage) = 0;

	protected:

		map<T, PacketType> CommandPacketTypeMap;

	};

}
}





#endif