#include "FileReader.h"

#include "../../../Util/FileSystem.h"
#include "../../../Util/Log.h"
#include <string.h>
#include <iostream>


using namespace Base::Sheetmusics::IO;
using namespace Util;
using namespace std;


FileReader::FileReader(string p)
{
	path = p;
}

ifstream * FileReader::GetStream(string name)
{
	return new ifstream(path + "/" + name);
}

SmSetInfo * FileReader::GetSmSetInfo()
{
	SmSetInfo* smSetInfo = new SmSetInfo();
	smSetInfo->path = path;
	return smSetInfo;
}

vector<string>* FileReader::WhereEndWith(string s)
{

	LOG(LogLevel::Info) << "FileReader::WhereEndWith(string) : read all sheetmusics under [" << s << "].";

	char*** fileNames = new char**();

	int fileCount = GetFiles(path.c_str(), fileNames);

	vector<string>* files = new vector<string>();



	for (int i = 0; i < fileCount; i++) {

		int len = strlen((*fileNames)[i]);
		
		LOG(LogLevel::Debug) << "FileReader::WhereEndWith : check if [" << (*fileNames)[i] << "] is .sm ";

		if (len >= 3 && strcmp((*fileNames)[i] + len - 3, ".sm") == 0) {

			LOG(LogLevel::Info) << "FileReader::WhereEndWith : sheetmusic read : [" << (*fileNames)[i] << "]";
			files->push_back((*fileNames)[i]);

		}
		else if (len >= 3){
			LOG(LogLevel::Debug) << "FileReader::WhereEndWith : not sm : [" << (*fileNames)[i] << "]. the last words are ["
				<< (*fileNames)[i] + len - 3 << "], the return of strcmp is [" << strcmp((*fileNames)[i] + len - 3, ".sm") << "]";
		}
	}

	// delete記憶體，但其實記憶體是連在一起的，應該只要free第一個元素就好
	//for (int i = 0; i < fileCount; i++) {
	//	free((*fileNames)[i]);
	//}
	if(fileCount > 0)
		free((*fileNames)[0]);


	free(*fileNames);
	delete fileNames;

	LOG(LogLevel::Finer) << "FileReader::WhereEndWith : function ends.";
	return files;
}
