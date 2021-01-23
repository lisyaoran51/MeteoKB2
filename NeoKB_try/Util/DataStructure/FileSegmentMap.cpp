#include "FileSegmentMap.h"

#include <string.h>


using namespace Util::DataStructure;


FileSegmentMap::FileSegmentMap(int sSize)
{
	segmentSize = sSize;
}

FileSegmentMap::~FileSegmentMap()
{
	for (map<int, pair<char*, int>>::const_iterator it = fileSegmentMap.begin(); it != fileSegmentMap.end(); ++it)
	{
		delete[] it->second.first;
	}
	fileSegmentMap.clear();
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

int FileSegmentMap::GetSegmentSize()
{
	return segmentSize;
}

bool FileSegmentMap::CheckFullFilled()
{
	if (segmentAmount == fileSegmentMap.size())
		return true;
	return false;
}

int FileSegmentMap::WriteFile(fstream * fStream)
{
	for (map<int, pair<char*, int>>::const_iterator it = fileSegmentMap.begin(); it != fileSegmentMap.end(); ++it)
	{
		fStream->write(it->second.first, it->second.second * sizeof(char*));
	}

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
	segmentAmount = fileSize / segmentSize + fileSize % segmentSize > 0 ? 1 : 0;

	/* 開始讀黨 */
	fStream->seekp(0, ios_base::beg);
	for (int i = 0; i < segmentAmount; i++) {

		char* buffer = nullptr;
		int bufferSize = 0;

		if (i == segmentAmount - 1 && fileSize % segmentSize != 0)
			bufferSize = fileSize % segmentSize;
		else
			bufferSize = segmentSize;

		buffer = new char[bufferSize];
		fStream->read(buffer, bufferSize);
		fileSegmentMap[i] = pair<char*, int>(buffer, bufferSize);

	}

	return 0;
}
