#ifndef PLATFORM_STORAGE_H
#define PLATFORM_STORAGE_H


#include "Storage.h"



namespace Framework {
namespace IO{


	/// <summary>
	/// �A�U���x�i�H�Ϊ�storage�A�ثe�ɶq��c++���ت�lib�g���AlocateBasePath�ثe�u��linux�i�H��
	/// </summary>
	class PlatformStorage : public Storage {


	public:

		PlatformStorage(string bName, string sDirectory = "");

		/// <summary>
		/// �d�ݦ��S���o���ɮ�
		/// </summary>
		virtual bool Exist(string filePath);

		/// <summary>
		/// �d�ݦ��S���o�Ӹ�Ƨ�
		/// </summary>
		virtual bool ExistDirectory(string directoryPath);

		/// <summary>
		/// �R��
		/// </summary>
		virtual int Delete(string filePath);

		/// <summary>
		/// �R��Ƨ��M�U���Ҧ��F��
		/// </summary>
		virtual int DeleteDirectory(string directoryPath);

		/// <summary>
		/// ��X�Ҧ���Ƨ��A�Χ��n��vector�^��
		/// </summary>
		virtual vector<string>* GetDirectories(string directoryPath);

		virtual fstream* GetStream(string filePath);

		virtual fstream* GetStream(string filePath, bool volitile, bool binary = false);

		/// <summary>
		/// �|�۰ʫظ�Ƨ��M�ɮ�
		/// �g�J�M�ΡA���`���n�γo��
		/// </summary>
		virtual fstream* GetStream(string filePath, FileAccess fileAccess, FileMode fileMode, bool binary = false);


	protected:

		virtual string locateBasePath();

		virtual string locateWriteBasePath();

		virtual Storage* clone(string newSubDirectory = "");


	};


}}







#endif