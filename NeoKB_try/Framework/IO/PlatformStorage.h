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

		PlatformStorage(string bName);

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
		/// ��X�Ҧ���Ƨ�
		/// </summary>
		virtual vector<string>* GetDirectories(string directoryPath);

		virtual ifstream GetStream(string filePath);

	protected:

		virtual string locateBasePath();

		virtual Storage* clone(string subDirectory);


	};


}}







#endif