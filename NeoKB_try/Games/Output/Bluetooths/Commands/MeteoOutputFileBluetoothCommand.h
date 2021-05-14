#ifndef METEO_OUTPUT_FILE_BLUETOOTH_COMMAND_H
#define METEO_OUTPUT_FILE_BLUETOOTH_COMMAND_H


#include "../../../Input/Commands/MeteoBluetoothCommand.h"
#include <fstream>


using namespace Games::Input::Commands;
using namespace std;


namespace Games {
namespace Output{
namespace Bluetooths{
namespace Commands{


	class MeteoOutputFileBluetoothCommand : public MeteoBluetoothCommand {

		int segmentSize = 512;

	public:

		MeteoOutputFileBluetoothCommand(MeteoCommand c, json text);

		MeteoOutputFileBluetoothCommand(MeteoCommand c);

		MeteoOutputFileBluetoothCommand(MeteoCommand c, string p, int sSize = 512);

		virtual ~MeteoOutputFileBluetoothCommand();

		int GetFileSegment(int segmentNumber, char* fileSegment);

		/// <summary>
		/// 拿總共有多少片段
		/// <//summary>
		int GetFileSegmentCount();

		/// <summary>
		/// 拿每一個片段的大小是多少，通常是512
		/// <//summary>
		int GetFileSegmentSize();

		int CloseStream();

		string GetFileName();

		string GetPath();

		int SetPath(string p);

		int AddSegmentToReOutput(int segmentNumber);

		int DeleteSegmentToReOutput(int segmentNumber);

		vector<int>& GetSegmentsToReOutput();

		int SetIsOutputSuccess(bool isSuccess);

		bool GetIsOutputSuccess();

	protected:

		fstream* file = nullptr;

		int fileSize = 0;

		int tempSegmentNumber = 0;

		int maxSegmentCount = 0;

		bool isStreamOpen = false;

		string path = "";

		string fileName = "";

		vector<int> segmentsToReOutput;

		bool isOutputSuccess = false;

	};

}}}}





#endif