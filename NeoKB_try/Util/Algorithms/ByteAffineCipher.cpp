#include "ByteAffineCipher.h"

#include <stdlib.h> /* 亂數相關函數 */
#include <time.h>   /* 時間相關函數 */
#include <iostream>
#include <fstream>

using namespace Util::Algorithms;
using namespace std;


ByteAffineCipher::ByteAffineCipher()
{
	initializeDefaultTable();
}

ByteAffineCipher::ByteAffineCipher(int tCount, unsigned char * aTables, unsigned char * oTable)
{
	tableCount = tableCount;
	affineTables = aTables;
	offsetTable = oTable;
}

ByteAffineCipher::ByteAffineCipher(int tCount, unsigned char * aTables, unsigned char * dTables, unsigned char * oTable)
{
	tableCount = tableCount;
	affineTables = aTables;
	deaffineTables = dTables;
	offsetTable = oTable;
}

unsigned char * ByteAffineCipher::Encrypt(unsigned char * originalData, int dataSize)
{
	int offset = (int)offsetTable[dataSize % 256];

	unsigned char* affineTable = &affineTables[dataSize % 256 * 256];

	unsigned char* newFileData = new unsigned char[dataSize];

	for (int i = 0; i < dataSize; i++) {

		int originalBytePosition = i + offset >= dataSize ? i + offset - dataSize : i + offset;

		newFileData[i] = affineTable[(int)originalData[originalBytePosition]];

	}


	return newFileData;
}

unsigned char * ByteAffineCipher::Decrypt(unsigned char * originalData, int dataSize)
{
	int offset = (int)offsetTable[dataSize % 256];

	unsigned char* deaffineTable = &deaffineTables[dataSize % 256 * 256];

	unsigned char* newFileData = new unsigned char[dataSize];

	for (int i = 0; i < dataSize; i++) {

		int originalBytePosition = i - offset < 0 ? i - offset + dataSize : i - offset;

		newFileData[i] = deaffineTable[(int)originalData[originalBytePosition]];

	}


	return nullptr;
}

int ByteAffineCipher::GenerateAffineOffsetTable(string path)
{
	unsigned char* newAffineTables = new unsigned char[256 * 256];
	unsigned char* newDeaffineTables = new unsigned char[256 * 256];
	
	/* 固定亂數種子 */
	srand(time(NULL));

	int tempOffset = rand() % 256;

	for (int i = 0; i < 256; i++) {
		unsigned char tempAffineTable[256] = { 0 };
		unsigned char tempDeaffineTable[256] = { 0 };

		for (int j = 0; j < 256; j++) {

			unsigned char thisRandomCode;
			bool isCollided = false;
			do{
				thisRandomCode = (unsigned char)rand() % 256;

				for (int k = 0; k < j; k++) {
					if (thisRandomCode == tempAffineTable[k])
						isCollided = true;
				}

			}while (isCollided);

			tempAffineTable[j] = thisRandomCode;
			tempDeaffineTable[thisRandomCode] = j;
		}

		memcpy(&newAffineTables[i * 256], tempAffineTable, 256 * sizeof(unsigned char));
		memcpy(&newDeaffineTables[i * 256], tempDeaffineTable, 256 * sizeof(unsigned char));

	}

	ofstream myfile;
	myfile.open("example.txt");

	myfile << string("Offset:") << tempOffset << string("\n affin table { \n");

	for (int i = 0; i < 256; i++) {

		for (int j = 0; j < 256; j++) {

			myfile << (int)newAffineTables[i * 256 + j] << " ";
			if (j % 32 == 0)
				myfile << "\n";
		}

	}
	myfile << "}\n\n deaffine table { \n";


	for (int i = 0; i < 256; i++) {

		for (int j = 0; j < 256; j++) {

			myfile << (int)newDeaffineTables[i * 256 + j] << " ";
			if (j % 32 == 0)
				myfile << "\n";
		}

	}
	myfile << "}";

	myfile.close();

	return 0;
}

int ByteAffineCipher::initializeDefaultTable()
{
	return 0;
}
