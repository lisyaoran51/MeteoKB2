#include "FileSegmentMap.h"

#include <string.h>
#include "../StringSplitter.h"
#include "../Log.h"


using namespace Util::DataStructure;
using namespace Util;


FileSegmentMap::FileSegmentMap(int sSize)
{
	maxSegmentSize = sSize;
}

FileSegmentMap::~FileSegmentMap()
{
	for (map<int, pair<char*, int>>::const_iterator it = fileSegmentMap.begin(); it != fileSegmentMap.end(); ++it)
	{
		delete[] it->second.first;
	}
	fileSegmentMap.clear();
}

string FileSegmentMap::GetFileNameWithoutExtension()
{
	vector<string> splitFileName = StringSplitter::Split(fileName, ".");

	return splitFileName[0];
}

int FileSegmentMap::GetFileSegment(int index, char ** fileSegment)
{
	if (fileSegmentMap.find(index) != fileSegmentMap.end()) {
		*fileSegment = new char[fileSegmentMap[index].second];
		memcpy(*fileSegment, fileSegmentMap[index].first, fileSegmentMap[index].second);
		return fileSegmentMap[index].second;
		
	}
	else {
		return -1;
	}

	return 0;
}

int FileSegmentMap::GetMaxSegmentSize()
{
	return maxSegmentSize;
}

int FileSegmentMap::GetSegmentAmount()
{
	return segmentAmount;
}

bool FileSegmentMap::CheckFullFilled()
{
	if (segmentAmount == fileSegmentMap.size())
		return true;
	return false;
}

int FileSegmentMap::WriteFile(fstream * fStream)
{
	for (int i = 0; i < segmentAmount; i++) {

		LOG(LogLevel::Debug) << "FileSegmentMap::WriteFile() : write file [" << string(fileSegmentMap[i].first) << "] with size [" << fileSegmentMap[i].second << "] to file.";
		fStream->write(fileSegmentMap[i].first, fileSegmentMap[i].second * sizeof(char));
	}

	//for (map<int, pair<char*, int>>::const_iterator it = fileSegmentMap.begin(); it != fileSegmentMap.end(); ++it)
	//{
	//	fStream->write(it->second.first, it->second.second * sizeof(char));
	//}

	return 0;
}

int FileSegmentMap::ReadFile(fstream * fStream)
{
	/* 計算檔案大小 */
	fStream->seekp(0, ios_base::beg);
	streampos fileSize = fStream->tellg();
	fStream->seekg(0, std::ios::end);
	fileSize = fStream->tellg() - fileSize;

	/* 計算segment數量 */
	segmentAmount = fileSize / maxSegmentSize + fileSize % maxSegmentSize > 0 ? 1 : 0;
	//segmentAmount = fileSize / segmentSize + (fileSize % segmentSize > 0 ? 1 : 0);

	/* 開始讀黨 */
	fStream->seekp(0, ios_base::beg);
	for (int i = 0; i < segmentAmount; i++) {

		char* buffer = nullptr;
		int bufferSize = 0;

		if (i == segmentAmount - 1 && fileSize % maxSegmentSize != 0)
			bufferSize = fileSize % maxSegmentSize;
		else
			bufferSize = maxSegmentSize;

		buffer = new char[bufferSize];
		fStream->read(buffer, bufferSize);
		fileSegmentMap[i] = pair<char*, int>(buffer, bufferSize);

	}

	return 0;
}

int FileSegmentMap::Erase()
{
	for (int i = 0; i < segmentAmount; i++) {
		delete[] fileSegmentMap[i].first;
	}

	fileSegmentMap.clear();
	return 0;
}
