#ifndef SHEETMUSIC_FILE_INFO_H
#define SHEETMUSIC_FILE_INFO_H


#include "../../Framework/IO/FileInfo.h"


using namespace Framework::IO;


namespace Games {
namespace Sheetmusics{

	class SheetmusicFileInfo {

	public:

		int Id = -1;

		int SheetmusicId = -1;

		int FileInfoId = -1;

		FileInfo* FileInfo = nullptr;

		string FileName = "";

	};


}}




#endif