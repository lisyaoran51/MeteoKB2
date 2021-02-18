#ifndef METEO_PACKET_CONVERTER_V2_H
#define METEO_PACKET_CONVERTER_V2_H


#include "PacketConverter.h"
#include "../../Games/Output/Bluetooths/MeteoBluetoothMessage.h"
#include <vector>
#include <map>
#include "../../Framework/IO/Storage.h"
#include <utility>


using namespace std;
using namespace Framework::IO;
using namespace Games::Output::Bluetooths;


namespace Desktop {
namespace Devices{

	class MeteoPacketConverterV2 : public PacketConverter<MeteoCommand> {

		const int maxPacketLength = 156;

		const int maxFileSegmentSize = 128;


		char contextBuffer[1024] = { 0 };

		/// <summary>
		/// �W�@��split packet���ɭԡA�̧��ڦp�G���ѤU�Q�I�_��packet�A�N�s��o�̭��A�U��split packet���ɭԴN�b��o�ӱ���U�@����buffer���Y
		/// </summary>
		char lastBufferSegment[2048] = { 0 };

		/// <summary>
		/// �W�@��split packet���ɭԡA�̧��ڦ��ѤU�Q�I�_��packet��size�C�p�G�S���ѴN�O0
		/// </summary>
		int lastBufferSegmentSize = 0;

		map<unsigned long, MeteoCommand> commandMap;

		string getFileName(char* buffer, int size);

		int getFileSize(char* buffer, int size);

		char* getFileSegment(char* buffer, int size);

		int getFileSegmentOrder(char* buffer, int size);

		int getFileSegmentCount(char* buffer, int size);


	public:

		MeteoPacketConverterV2();

		virtual int SplitPacket(char* bufferIn, int bytesRead, char** packets, int* packerLengths);

		virtual PacketStatus CheckPacketStatus(char* packet, int length);

		virtual PacketType CheckPacketType(char* buffer, int size);

		virtual PacketType CheckCommandType(BluetoothMessage* bluetoothMessage);

		virtual BluetoothMessage* ConvertToBluetoothMessage(char* buffer, int size);

		virtual int GetCountOfPacket(BluetoothMessage* bluetoothMessage);

		virtual int ConvertToByteArray(BluetoothMessage* bluetoothMessage, int packetOrder, char* buffer, int bufferMaxSize);
		
		/// <summary>
		/// �^�ǭȬO�ഫ�n��file segment
		/// <//summary>
		virtual BluetoothMessage* ConvertToFile(char* buffer, int size);



	};


}}



#endif