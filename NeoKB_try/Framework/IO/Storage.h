#ifndef STORAGE_H
#define STORAGE_H


#include <string>
#include <vector>
#include <fstream>
#include "../Allocation/Hierachal/MtoObject.h"
#include "FileAccess.h"
#include "FileMode.h"


using namespace std;
using namespace Framework::Allocation::Hierachal;


namespace Framework {
namespace IO{


	class Storage: public MtoObject {


	public:

		int Initialize();
		
		/// <summary>
		/// ���J���|�Mstrorage�����|��b�@�_�^�ǡA�u���J��J��Ƨ����|
		/// �n�O�o�}�Y���঳/�A�|�X��
		/// </summary>
		virtual string GetUsableDirectoryPathFor(string directoryPath, bool createIfNotExist = false);

		/// <summary>
		/// ���J���|�Mstrorage�����|��b�@�_�^�ǡA�u���J��J�ɮ׸��|
		/// �n�O�o�}�Y���঳/�A�|�X��
		/// </summary>
		virtual string GetUsableFilePathFor(string directoryPath, bool createIfNotExist = false);

		/// <summary>
		/// �d�ݦ��S���o���ɮ�
		/// </summary>
		virtual bool Exist(string filePath) = 0;

		/// <summary>
		/// �d�ݦ��S���o�Ӹ�Ƨ�
		/// </summary>
		virtual bool ExistDirectory(string directoryPath) = 0;

		/// <summary>
		/// �R��
		/// </summary>
		virtual int Delete(string filePath) = 0;

		/// <summary>
		/// �R��Ƨ��M�U���Ҧ��F��
		/// </summary>
		virtual int DeleteDirectory(string directoryPath) = 0;

		/// <summary>
		/// ��X�Ҧ���Ƨ�
		/// </summary>
		virtual vector<string>* GetDirectories(string directoryPath) = 0;

		Storage* GetStorageForDirectory(string directoryPath);

		/// <summary>
		/// �ϥΫe�n��check exist�A���M�|�X��
		/// �o�ӬOread only���A�p�G�n�g�J�n�γ̤U������
		/// </summary>
		virtual fstream* GetStream(string filePath) = 0;

		virtual fstream* GetStream(string filePath, bool volitile, bool binary = false) = 0;

		/// <summary>
		/// �|�۰ʫظ�Ƨ��M�ɮ�
		/// �g�J�M�ΡA���`���n�γo��
		/// </summary>
		virtual fstream* GetStream(string filePath, FileAccess fileAccess, FileMode fileMode, bool binary = false) = 0;

	protected:

		bool initialized = false;

		/// <summary>
		/// �q�t�ιw�]�����|�W���A���w�@�Ӹ�Ƨ� �e�᳣���঳�׽u
		/// </summary>
		string baseName = "";

		/// <summary>
		/// �q�t�ιw�]�����| �e�᳣���঳�׽u
		/// </summary>
		string basePath = "";

		Storage(string bName, string sDirectory = "");

		/// <summary>
		/// �e�᳣���঳�׽u
		/// </summary>
		string subDirectory;

		virtual string locateBasePath() = 0;

		virtual string locateWriteBasePath() = 0;

		/// <summary>
		/// �e�᳣���঳�׽u�A�|�{�b����subdirectory�U���A�Ť@��subDirectory
		/// </summary>
		virtual Storage* clone(string newSubDirectory = "") = 0;


	};


}}





#endif