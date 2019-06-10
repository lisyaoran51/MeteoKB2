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
		/// �s�y�@���s���A����o�Ӧbsminfo�̭��P��
		/// </summary>
		SmSetInfo* GetSmSetInfo();

		/// <summary>
		/// ��X�b�ثe���|�U�Ҧ��ɦW������s���W�r
		/// </summary>
		vector<string>* WhereEndWith(string s);
	};

}}}





#endif