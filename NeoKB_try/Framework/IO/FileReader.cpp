#include "FileReader.h"

#include "../../Util/FileSystem.h"
#include "../../Util/Log.h"
#include <string.h>
#include <iostream>


using namespace Framework::IO;
using namespace Util;
using namespace std;
using namespace std::literals::string_literals;


FileReader::FileReader(string p)
{
	if(p.substr(p.length() - 1, 1) == "/"s)
		p = p.substr(0, p.length() - 1);

	path = p;
}

fstream * FileReader::GetStream(string name)
{
	return new fstream(path + "/" + name);
}

string FileReader::GetPath()
{
	return path;
}

string FileReader::GetPath(string name)
{
	return path + "/"s + name;
}

vector<string>* FileReader::WhereEndWith(string s)
{

	LOG(LogLevel::Info) << "FileReader::WhereEndWith(string) : read all files with extension [" << s << "].";

	char*** fileNames = new char**();

	int fileCount = GetFiles(path.c_str(), fileNames);

	vector<string>* files = new vector<string>();

	string extension = s;
	if (extension[0] != '.') {
		extension = string(".") + extension;
	}


	for (int i = 0; i < fileCount; i++) {

		int len = strlen((*fileNames)[i]);
		
		LOG(LogLevel::Finer) << "FileReader::WhereEndWith : check if [" << (*fileNames)[i] << "] is .sm ";

		if (len >= extension.size() && strcmp((*fileNames)[i] + len - extension.size(), extension.c_str()) == 0) {

			LOG(LogLevel::Fine) << "FileReader::WhereEndWith : " << extension << " read : [" << (*fileNames)[i] << "]";
			files->push_back((*fileNames)[i]);

		}
		else if (len >= extension.size()){
			LOG(LogLevel::Finer) << "FileReader::WhereEndWith : not " << extension << " : [" << (*fileNames)[i] << "]. the last words are ["
				<< (*fileNames)[i] + len - extension.size() << "], the return of strcmp is [" << strcmp((*fileNames)[i] + len - extension.size(), extension.c_str()) << "]";
		}
	}

	// delete�O����A�����O����O�s�b�@�_���A���ӥu�nfree�Ĥ@�Ӥ����N�n
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