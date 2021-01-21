#ifndef METEO_PACKET_CONVERTER_V1_H
#define METEO_PACKET_CONVERTER_V1_H


#include "PacketConverter.h"
#include "../../Games/Output/Bluetooths/MeteoBluetoothMessage.h"
#include <vector>
#include <map>
#include "../../Framework/IO/Storage.h"
#include <utility>


using namespace std;
using namespace Games::Input::Commands;
using namespace Framework::IO;
using namespace Games::Output::Bluetooths;


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

		/// <summary>
		/// 上一次split packet的時候，最尾巴如果有剩下被截斷的packet，就存到這裡面，下次split packet的時候就在把這個接到下一次的buffer的頭
		/// </summary>
		char lastBufferSegment[2048] = { 0 };

		/// <summary>
		/// 上一次split packet的時候，最尾巴有剩下被截斷的packet的size。如果沒有剩就是0
		/// </summary>
		int lastBufferSegmentSize = 0;

		map<unsigned long, MeteoCommand> commandMap;

		string getFileName(char* buffer, int size);

		int getFileSize(char* buffer, int size);

		char* getFileSegment(char* buffer, int size);

		int getFileSegmentOrder(char* buffer, int size);

		int getFileSegmentCount(char* buffer, int size);

		MeteoPacketConverterFileType getFileType(char* buffer, int size);

		MeteoBluetoothMessage* createAckFileSegmentBluetoothMessage(char* buffer, int size);


	public:

		MeteoPacketConverterV1(Storage* s);

		virtual int SplitPacket(char* bufferIn, int bytesRead, char** packets, int* packerLengths);

		virtual PacketStatus CheckPacketStatus(char* packet, int length);

		virtual PacketType CheckPacketType(char* buffer, int size);

		virtual PacketType CheckCommandType(BluetoothMessage* bluetoothMessage);

		virtual BluetoothMessage* ConvertToBluetoothMessage(char* buffer, int size);

		//virtual MeteoBluetoothCommand* ConvertToBluetoothCommand(BluetoothMessage* bluetoothMessage);

		virtual int GetCountOfPacket(BluetoothMessage* bluetoothMessage);

		virtual int ConvertToByteArray(BluetoothMessage* bluetoothMessage, int packetOrder, char* buffer, int bufferMaxSize);
		
		/// <summary>
		/// 回傳值就是收到訊息的ack訊息，直接傳回手機就好
		/// <//summary>
		virtual BluetoothMessage* ConvertToFile(char* buffer, int size);

		virtual bool CheckIsFinishWriteCommand(BluetoothMessage* bluetoothMessage);

		virtual BluetoothMessage* FinishWriteFile(BluetoothMessage* bluetoothMessage);

		virtual bool CheckIsWrtieFileFinishCommand(BluetoothMessage* bluetoothMessage);

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