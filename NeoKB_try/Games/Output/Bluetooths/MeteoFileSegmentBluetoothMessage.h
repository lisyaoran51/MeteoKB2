#ifndef METEO_FILE_SEGMENT_BLUETOOTH_MESSAGE_H
#define METEO_FILE_SEGMENT_BLUETOOTH_MESSAGE_H


#include "MeteoBluetoothMessage.h"


namespace Games {
namespace Output {
namespace Bluetooths {

	class MeteoFileSegmentBluetoothMessage : public MeteoBluetoothMessage {

	public:

		/// <summary>
		/// 會把丟進去的buffer給複製進去
		/// </summary>
		MeteoFileSegmentBluetoothMessage(MeteoCommand mCommand, char* fSegment, int fSegmentSize, string fName, int o, int a);

		virtual BluetoothMessage* Clone();

		virtual int SetContext(char* c, int cSize);

		virtual int SetAccessType(MeteoBluetoothMessageAccessType aType);

		string GetFileName();

		/// <summary>
		/// 會把裡面的array指標回傳出來
		/// </summary>
		char* GetFileSegment();

		/// <summary>
		/// 會複製一份file segment出來
		/// </summary>
		int GetFileSegment(char** fSegment);

		int GetFileSegmentSize();

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

		char* fileSegment = nullptr;

		/// <summary>
		/// 
		/// </summary>
		int fileSegmentSize = -1;

	};

}}}





#endif