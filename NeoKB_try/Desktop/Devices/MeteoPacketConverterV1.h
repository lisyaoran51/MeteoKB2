#ifndef METEO_PACKET_CONVERTER_V1_H
#define METEO_PACKET_CONVERTER_V1_H


#include "PacketConverter.h"
#include "../../Games/Input/Commands/MeteoBluetoothCommand.h"
#include <vector>
#include <map>
#include "../../Framework/IO/Storage.h"
#include <utility>


using namespace std;
using namespace Games::Input::Commands;
using namespace Framework::IO;


namespace Desktop {
namespace Devices{

	class MeteoPacketConverterV1 : public PacketConverter<MeteoCommand> {

		const int maxPacketLength = 538;

		const int maxFileSegmentSize = 512;

	protected:
		enum class MeteoPacketConverterFileType;
		class MeteoPacketConverterFileSegmentMap;

	private:

		char contextBuffer[1024] = { 0 };

		map<unsigned long, MeteoCommand> commandMap;

		string getFileName(char* buffer, int size);

		int getFileSize(char* buffer, int size);

		char* getFileSegment(char* buffer, int size);

		int getFileSegmentOrder(char* buffer, int size);

		int getFileSegmentCount(char* buffer, int size);

		MeteoPacketConverterFileType getFileType(char* buffer, int size);

		MeteoBluetoothCommand* createAckFileSegmentBluetoothCommand(char* buffer, int size);


	public:

		MeteoPacketConverterV1(Storage* s);

		virtual int SplitPacket(char* bufferIn, int bytesRead, char** packets, int* packerLengths);

		virtual PacketStatus CheckPacketStatus(char* packet, int length);

		virtual PacketType CheckPacketType(char* buffer, int size);

		virtual PacketType CheckCommandType(BluetoothCommand* bluetoothCommand);

		virtual BluetoothCommand* ConvertToBluetoothCommand(char* buffer, int size);

		virtual MeteoBluetoothCommand* ConvertToBluetoothCommand(BluetoothMessage* bluetoothMessage);

		virtual int GetCountOfPacket(BluetoothCommand* bluetoothCommand);

		virtual int ConvertToByteArray(BluetoothCommand* bluetoothCommand, int packetOrder, char* buffer, int bufferMaxSize);
		
		/// <summary>
		/// 回傳值就是收到訊息的ack訊息，直接傳回手機就好
		/// <//summary>
		virtual MeteoBluetoothCommand* ConvertToFile(char* buffer, int size);

		virtual bool CheckIsFinishWriteCommand(BluetoothCommand* bluetoothCommand);

		virtual BluetoothCommand* FinishWriteFile(BluetoothCommand* bluetoothCommand);

		virtual bool CheckIsWrtieFileFinishCommand(BluetoothCommand* bluetoothCommand);

	protected:

		Storage* storage = nullptr;

		map<string, MeteoPacketConverterFileSegmentMap*> fileMap;

		int writeFile(MeteoPacketConverterFileSegmentMap* file, string path);

		enum class MeteoPacketConverterFileType {
			None,
			Sheetmusic,
			Sound,
			Program,
		};

		class MeteoPacketConverterFileSegmentMap {
		public:
			~MeteoPacketConverterFileSegmentMap();

			string fileName;
			map<int, pair<char*, int>> fileSegmentMap;
			int segmentAmount;
			MeteoPacketConverterFileType fileType;
			bool CheckFullFilled();
			int WriteFile(fstream* fStream);
		};

	};


}}



#endif