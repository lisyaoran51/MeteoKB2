#ifndef METEO_FILE_BLUETOOTH_COMMAND_H
#define METEO_FILE_BLUETOOTH_COMMAND_H


#include "MeteoBluetoothCommand.h"


namespace Games {
namespace Input{
namespace Commands{

	/// <summary>
	/// 預設最多重連三次
	/// </summary>
	class MeteoFileBluetoothCommand : public MeteoBluetoothCommand {

	public:

		MeteoFileBluetoothCommand(MeteoCommand c);

		MeteoFileBluetoothCommand(MeteoCommand c, int o, string fName);

		int SetOrder(int o);

		int GetOrder();

		int SetFileName(string fName);

		string GetFileName();

		/// <summary>
		/// 回傳segment的size，給出去的binary是複製出來的，所以這邊在解構的時候可以把binary刪掉
		/// </summary>
		int GetBinarySegment(char** binarySegmentPointer);

		virtual BluetoothCommand* Clone();

	protected:

		int order = -1;

		string fileName = "";

	};

}}}



#endif