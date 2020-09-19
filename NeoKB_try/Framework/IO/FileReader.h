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
		/// ��X�b�ثe���|�U�Ҧ��ɦW������s���W�r
		/// </summary>
		vector<string>* WhereEndWith(string s);
	};

}}





#endif