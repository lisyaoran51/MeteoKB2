#include "DynamicPathResourceStore.h"

#include <experimental/filesystem>


using namespace Framework::IO::Stores;
using namespace std;
using namespace std::experimental::filesystem;


DynamicPathResourceStore::DynamicPathResourceStore(string p): RegisterType("DynamicPathResourceStore")
{
	path = p;
}

char* DynamicPathResourceStore::Get(string name)
{
	string filePath = path + "/" + name;

	ifstream ifs(filePath);



	if (!ifs) {
		string message = string("DynamicPathResourceStore cannot found \"") + name + string("\"");
		throw FileNotFoundException((char*)message.c_str());
	}

	// 這一段是往路找到的
	// https://stackoverflow.com/questions/2912520/read-file-contents-into-a-string-in-c

	string content((istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>()));
	char* contentInChar = (char*)content.c_str();

	return contentInChar;
}

ifstream * DynamicPathResourceStore::GetStream(string name)
{

	string filePath = path + "/" + name;

	ifstream* ifs = new ifstream(filePath);



	if (!(*ifs)) {
		delete ifs;
		return nullptr;
	}

	return ifs;
}

string DynamicPathResourceStore::GetFilePath(string name)
{
	string filePath = path + "/" + name;

	if (exists(filePath)) {

		return filePath;
	}
	else
		throw runtime_error("DynamicPathResourceStore::GetFilePath(): file not found.");
	return string();
}
