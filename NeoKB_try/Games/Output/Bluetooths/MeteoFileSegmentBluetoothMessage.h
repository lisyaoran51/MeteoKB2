#ifndef METEO_FILE_SEGMENT_BLUETOOTH_MESSAGE_H
#define METEO_FILE_SEGMENT_BLUETOOTH_MESSAGE_H


#include "MeteoBluetoothMessage.h"


namespace Games {
namespace Output {
namespace Bluetooths {

	class MeteoFileSegmentBluetoothMessage : public MeteoBluetoothMessage {

	public:

		/// <summary>
		/// �|���i�h��buffer���ƻs�i�h
		/// </summary>
		MeteoFileSegmentBluetoothMessage(MeteoCommand mCommand, char* fSegment, int fSegmentSize, string fName, int o, int a);

		virtual BluetoothMessage* Clone();

		virtual int SetContext(char* c, int cSize);

		virtual int SetAccessType(MeteoBluetoothMessageAccessType aType);

		string GetFileName();

		/// <summary>
		/// �|�ƻs�@��file segment�X��
		/// </summary>
		int GetFileSegment(char** fSegment);

		/// <summary>
		/// �o�ӬO�ĴX�q
		/// </summary>
		int GetOrder();

		/// <summary>
		/// �`�@���X�q�ɮ�
		/// </summary>
		int GetAmount();


	protected:

		string fileName;

		int order = -1;

		int amount = -1;

		char* fileSegment = nullptr;

		/// <summary>
		/// 
		/// </summary>
		int fileSegmentSize = -1;

	};

}}}





#endif