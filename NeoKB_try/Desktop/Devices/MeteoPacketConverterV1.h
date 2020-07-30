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

	protected:
		enum class MeteoPacketConverterFileType;
		class MeteoPacketConverterFileSegmentMap;

	private:

		map<unsigned long, MeteoCommand> commandMap;

		string getFileName(char* buffer, int size);

		int getFileSize(char* buffer, int size);

		char* getFileSegment(char* buffer, int size);

		int getFileSegmentNumber(char* buffer, int size);

		MeteoPacketConverterFileType getFileType(char* buffer, int size);


	public:

		MeteoPacketConverterV1(Storage* s);

		virtual PacketType CheckPacketType(char* buffer, int size);

		virtual BluetoothCommand* ConvertToBluetoothCommand(char* buffer, int size);

		virtual MeteoBluetoothCommand* ConvertToBluetoothCommand(BluetoothMessage* bluetoothMessage);

		virtual int ConvertToByteArray(BluetoothCommand* bluetoothCommand, char* buffer, int bufferMaxSize);
		
		virtual string ConvertToFile(char* buffer, int size);

	protected:

		Storage* storage = nullptr;

		map<string, MeteoPacketConverterFileSegmentMap*> fileMap;



		enum class MeteoPacketConverterFileType {
			None,
			Sheetmusic,
			Sound,
			Program,
		};

		class MeteoPacketConverterFileSegmentMap {
		public:
			string fileName;
			map<int, pair<char*, int>> fileSegmentMap;
			int segmentAmount;
			MeteoPacketConverterFileType fileType;
			bool CheckFullFilled();
			//WriteFile(storage);
		};

	};


}}



#endif