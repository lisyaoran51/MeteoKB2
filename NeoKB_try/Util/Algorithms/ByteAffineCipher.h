#ifndef AFFINE_CIPHER_H
#define AFFINE_CIPHER_H


#include <string>


using namespace std;



namespace Util {
namespace Algorithms{

	/// <summary>
	/// 用一個byte去做仿射，並且用offset去指定這個文件要向後offset多少。如何選擇仿射和偏移適用檔案大小去決定
	/// </summary>
	class ByteAffineCipher {

	public:

		/// <summary>
		/// 預設仿射表和偏移表
		/// </summary>
		ByteAffineCipher();

		ByteAffineCipher(int tCount, unsigned char* aTable, unsigned char* oTable);

		ByteAffineCipher(int tCount, unsigned char* aTables, unsigned char* dTable, unsigned char* oTable);

		unsigned char* Encrypt(unsigned char* originalData, int dataSize);

		unsigned char* Decrypt(unsigned char* originalData, int dataSize);

		int GenerateAffineOffsetTable(string path);


	protected:

		int tableCount = 256;

		unsigned char* affineTables = nullptr;

		unsigned char* deaffineTables = nullptr;

		unsigned char* offsetTable = nullptr;

		int initializeDefaultTable();

	};


}
}







#endif