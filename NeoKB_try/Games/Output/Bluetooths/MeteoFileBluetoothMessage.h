#ifndef METEO_BLUETOOTH_FILE_BLUETOOTH_MESSAGE_H
#define METEO_BLUETOOTH_FILE_BLUETOOTH_MESSAGE_H


#include "MeteoBluetoothMessage.h"


namespace Games {
namespace Output {
namespace Bluetooths {

	class MeteoFileBluetoothMessage : public MeteoBluetoothMessage {

	public:

		MeteoFileBluetoothMessage(MeteoCommand mCommand, string fPath);

		MeteoFileBluetoothMessage(MeteoCommand mCommand, string fPath, string fName);

		string GetFilePath();

		string GetFileName();

	protected:

		string filePath;

		string fileName;

	};

}}}





#endif