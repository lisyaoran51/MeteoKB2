#ifndef AFFINE_CIPHER_H
#define AFFINE_CIPHER_H


#include <string>


using namespace std;



namespace Util {
namespace Algorithms{

	/// <summary>
	/// �Τ@��byte�h����g�A�åB��offset�h���w�o�Ӥ��n�V��offset�h�֡C�p���ܥ�g�M�����A���ɮפj�p�h�M�w
	/// </summary>
	class ByteAffineCipher {

	public:

		/// <summary>
		/// �w�]��g��M������
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