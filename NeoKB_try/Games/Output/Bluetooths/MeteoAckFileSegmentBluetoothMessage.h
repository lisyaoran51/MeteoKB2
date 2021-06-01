#ifndef METEO_ACK_FILE_SEGMENT_BLUETOOTH_MESSAGE_H
#define METEO_ACK_FILE_SEGMENT_BLUETOOTH_MESSAGE_H


#include "MeteoBluetoothMessage.h"
namespace Games {
namespace Output{
namespace Bluetooths{



	class MeteoAckFileSegmentBluetoothMessage : public MeteoBluetoothMessage {

	public:

		/// <summary>
		/// 用來在收到檔案片段時回傳告知
		/// </summary>
		MeteoAckFileSegmentBluetoothMessage(MeteoCommand mCommand, string fName, int o, int a);

		virtual BluetoothMessage* Clone();

		virtual int SetContext(char* c, int cSize);

		virtual int SetAccessType(MeteoBluetoothMessageAccessType aType);

		string GetFileName();

		/// <summary>
		/// 這個是第幾段
		/// </summary>
		int GetOrder();

		/// <summary>
		/// 總共有幾段檔案
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