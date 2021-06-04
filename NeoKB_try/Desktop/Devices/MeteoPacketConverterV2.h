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

		unsigned short tempPacketId = 0;

		map<unsigned int, MeteoCommand> commandMap;

		string getFileName(const char* buffer, int size);

		int getFileSize(const char* buffer, int size);

		char* getFileSegment(const char* buffer, int size);

		int getFileSegmentOrder(const char* buffer, int size);

		int getFileSegmentCount(const char* buffer, int size);


	public:

		MeteoPacketConverterV2();

		virtual int SplitPacket(const char* bufferIn, int bytesRead, char** packets, int* packerLengths);

		virtual PacketStatus CheckPacketStatus(const char* packet, int length);

		virtual PacketType CheckPacketType(const char* buffer, int size);

		virtual PacketType CheckCommandType(BluetoothMessage* bluetoothMessage);

		virtual BluetoothMessage* ConvertToBluetoothMessage(const char* buffer, int size);

		virtual int GetCountOfPacket(BluetoothMessage* bluetoothMessage);

		virtual int ConvertToByteArray(BluetoothMessage* bluetoothMessage, int packetOrder, char* buffer, int bufferMaxSize);

		/// <summary>
		/// �^��array���סA�����\context message���׶W�L�@�ӫʥ]
		/// </summary>
		virtual int ConvertToByteArray(BluetoothMessage* bluetoothMessage, char* buffer, int bufferMaxSize);

		/// <summary>
		/// �^�ǭȬO�ഫ�n��file segment
		/// <//summary>
		virtual BluetoothMessage* ConvertToFile(const char* buffer, int size);

		/// <summary>
		/// �^�ǭȬOAck file
		/// <//summary>
		virtual BluetoothMessage* ConvertToAckFileMessage(const char* buffer, int size);



	};


}}



#endif