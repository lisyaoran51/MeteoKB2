#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <fstream>
#include <vector>
#include "../SheetmusicSetInfo.h"


using namespace std;
using namespace Base::Sheetmusics;


namespace Base {
namespace Sheetmusics {
namespace IO {

	class FileReader
	{
		string path;
	public:
		FileReader(string p);

		ifstream* GetStream(string name);

		/// <summary>
		/// 製造一份新的，之後這個在sminfo裡面銷毀
		/// </summary>
		SmSetInfo* GetSmSetInfo();

		/// <summary>
		/// 找出在目前路徑下所有檔名結尾式s的名字
		/// </summary>
		vector<string>* WhereEndWith(string s);
	};

}}}





#endif