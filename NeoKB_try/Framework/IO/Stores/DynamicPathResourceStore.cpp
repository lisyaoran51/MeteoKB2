#include "DynamicPathResourceStore.h"


using namespace Framework::IO::Stores;
using namespace std;


DynamicPathResourceStore::DynamicPathResourceStore(string p)
{
	path = p;
}

string DynamicPathResourceStore::Get(string name)
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

	return content;
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
