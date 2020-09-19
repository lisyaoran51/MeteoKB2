#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <fstream>
#include <vector>


using namespace std;


namespace Framework {
namespace IO {

	class FileReader
	{
		string path;
	public:
		FileReader(string p);

		fstream* GetStream(string name);

		string GetPath();

		string GetPath(string name);

		/// <summary>
		/// 找出在目前路徑下所有檔名結尾式s的名字
		/// </summary>
		vector<string>* WhereEndWith(string s);
	};

}}





#endif