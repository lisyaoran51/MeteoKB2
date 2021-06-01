#ifndef METEO_ACK_FILE_SEGMENT_BLUETOOTH_MESSAGE_H
#define METEO_ACK_FILE_SEGMENT_BLUETOOTH_MESSAGE_H


#include "MeteoBluetoothMessage.h"
namespace Games {
namespace Output{
namespace Bluetooths{



	class MeteoAckFileSegmentBluetoothMessage : public MeteoBluetoothMessage {

	public:

		/// <summary>
		/// �ΨӦb�����ɮפ��q�ɦ^�ǧi��
		/// </summary>
		MeteoAckFileSegmentBluetoothMessage(MeteoCommand mCommand, string fName, int o, int a);

		virtual BluetoothMessage* Clone();

		virtual int SetContext(char* c, int cSize);

		virtual int SetAccessType(MeteoBluetoothMessageAccessType aType);

		string GetFileName();

		/// <summary>
		/// �o�ӬO�ĴX�q
		/// </summary>
		int GetOrder();

		/// <summary>
		/// �`�@���X�q�ɮ�
		/// </summary>
		int GetAmount();

		virtual bool GetIsRawMessage();

	protected:

		string fileName;

		int order = -1;

		int amount = -1;

	};



}}}

#endif