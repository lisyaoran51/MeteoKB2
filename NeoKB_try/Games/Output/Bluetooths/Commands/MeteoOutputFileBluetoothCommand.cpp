#include "MeteoOutputFileBluetoothCommand.h"


using namespace Games::Output::Bluetooths::Commands;


MeteoOutputFileBluetoothCommand::MeteoOutputFileBluetoothCommand(MeteoCommand c, json text): MeteoBluetoothCommand(c, text)
{
	throw runtime_error("MeteoOutputFileBluetoothCommand::MeteoOutputFileBluetoothCommand(MeteoCommand, json) : this constructor is not available.");
}

MeteoOutputFileBluetoothCommand::MeteoOutputFileBluetoothCommand(MeteoCommand c): MeteoBluetoothCommand(c)
{
	throw runtime_error("MeteoOutputFileBluetoothCommand::MeteoOutputFileBluetoothCommand(MeteoCommand) : this constructor is not available.");
}

MeteoOutputFileBluetoothCommand::MeteoOutputFileBluetoothCommand(MeteoCommand c, string p, int sSize): MeteoBluetoothCommand(c)
{
	segmentSize = sSize;
	SetPath(p);

	/* 讀有幾個segment */
	file = new fstream(path, fstream::in | fstream::out | fstream::binary);
	isStreamOpen = true;

	long start, end;
	start = file->tellg();

	file->seekg(0, ios::end);
	end = file->tellg();
	fileSize = end - start;

	maxSegmentCount = fileSize / segmentSize + 1;

	file->seekg(0, ios::beg);

	tempSegmentNumber = 0;

	CloseStream();

	/* 先把需要重輸出的片段編號全列出來，之後在收到ack的時候再一個一個delete掉 */
	for (int i = 0; i < maxSegmentCount; i++)
		segmentsToReOutput.push_back(i);
}

MeteoOutputFileBluetoothCommand::~MeteoOutputFileBluetoothCommand()
{
	if (file != nullptr) {
		if (isStreamOpen) {
			file->close();
		}
		delete file;
	}
}

int MeteoOutputFileBluetoothCommand::GetFileSegment(int segmentNumber, char * fileSegment)
{

	/* 檔案已經讀完 */
	if (tempSegmentNumber >= tempSegmentNumber)
		return 0;

	if (tempSegmentNumber == segmentNumber) {	/* 讀取檔案中段 */
		int size = fileSize - tempSegmentNumber * segmentSize > segmentSize ? segmentSize : fileSize - tempSegmentNumber * segmentSize;

		fileSegment = new char[size];

		file->read(fileSegment, size);

		tempSegmentNumber++;

		return size;
	}
	else { /* 讀取檔案尾端 */
		int size = fileSize - segmentNumber * segmentSize > segmentSize ? segmentSize : fileSize - segmentNumber * segmentSize;

		file->seekg(segmentNumber * segmentSize, ios::beg);

		file->read(fileSegment, size);

		tempSegmentNumber = segmentNumber + 1;

		return size;
	}


	return 0;
}

int MeteoOutputFileBluetoothCommand::GetFileSegmentCount()
{
	return maxSegmentCount;
}

int MeteoOutputFileBluetoothCommand::GetFileSegmentSize()
{
	return segmentSize;
}

int MeteoOutputFileBluetoothCommand::CloseStream()
{
	if (!isStreamOpen)
		return -1;

	file->close();

	delete file;

	isStreamOpen = false;

	return 0;
}

string MeteoOutputFileBluetoothCommand::GetFileName()
{
	if (fileName != "")
		return fileName;

	/* 抓file name */
	string::size_type pos;

	string pathToSplit = path;
	string pattern = "/";
	pathToSplit += pattern;	//加一個在後面以方面操作
	int size = pathToSplit.size();


	string fName = "";

	for (int i = 0; i < size; i++)
	{
		pos = pathToSplit.find(pattern, i);
		if (pos < size)
		{
			string split = pathToSplit.substr(i, pos - i);
			fName = split;
			i = pos + pattern.size() - 1;
		}
	}

	fileName = fName;

	return fileName;
}

string MeteoOutputFileBluetoothCommand::GetPath()
{
	return path;
}

int MeteoOutputFileBluetoothCommand::SetPath(string p)
{
	path = p;

	/* 抓file name */
	string::size_type pos;

	string pathToSplit = path;
	string pattern = "/";
	pathToSplit += pattern;	//加一個在後面以方面操作
	int size = pathToSplit.size();


	string fName = "";

	for (int i = 0; i < size; i++)
	{
		pos = pathToSplit.find(pattern, i);
		if (pos < size)
		{
			string split = pathToSplit.substr(i, pos - i);
			fName = split;
			i = pos + pattern.size() - 1;
		}
	}

	fileName = fName;
	return 0;
}

int MeteoOutputFileBluetoothCommand::AddSegmentToReOutput(int segmentNumber)
{
	for (int i = 0; i < segmentsToReOutput.size(); i++) {
		if (segmentsToReOutput[i] == segmentNumber)
			return -1;
	}
	segmentsToReOutput.push_back(segmentNumber);

	return 0;
}

int MeteoOutputFileBluetoothCommand::DeleteSegmentToReOutput(int segmentNumber)
{
	for (int i = 0; i < segmentsToReOutput.size(); i++) {
		if (segmentsToReOutput[i] == segmentNumber) {
			segmentsToReOutput.erase(segmentsToReOutput.begin() + i);
		}
	}

	return 0;
}

vector<int>& MeteoOutputFileBluetoothCommand::GetSegmentsToReOutput()
{
	return segmentsToReOutput;
}

int MeteoOutputFileBluetoothCommand::SetIsOutputSuccess(bool isSuccess)
{
	isOutputSuccess = isSuccess;
	return 0;
}

bool MeteoOutputFileBluetoothCommand::GetIsOutputSuccess()
{
	return isOutputSuccess;
}
