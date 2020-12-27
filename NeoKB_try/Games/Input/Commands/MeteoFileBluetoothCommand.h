#ifndef METEO_FILE_BLUETOOTH_COMMAND_H
#define METEO_FILE_BLUETOOTH_COMMAND_H


#include "MeteoBluetoothCommand.h"


namespace Games {
namespace Input{
namespace Commands{

	/// <summary>
	/// �w�]�̦h���s�T��
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
		/// �^��segment��size�A���X�h��binary�O�ƻs�X�Ӫ��A�ҥH�o��b�Ѻc���ɭԥi�H��binary�R��
		/// </summary>
		int GetBinarySegment(char** binarySegmentPointer);

		virtual BluetoothCommand* Clone();

	protected:

		int order = -1;

		string fileName = "";

	};

}}}



#endif