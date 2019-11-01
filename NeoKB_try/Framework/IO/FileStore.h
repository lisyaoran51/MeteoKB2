#ifndef FILE_STORE_H
#define FILE_STORE_H

#include "../Database/DatabaseStore.h"
#include "Stores/ResourceStore.h"


using namespace Framework::Database;
using namespace Framework::IO::Stores;


namespace Framework {
namespace IO{

	/// <summary>
	/// file store�u�x�s�bfiles��Ƨ��U�����ɮ�
	/// �o�O�@�ӨS���Ϊ��F��A�s�bosu�̭����S����
	/// </summary>
	class FileStore : public DatabaseStore {

		ResourceStore<char*>* store;

	public:
		FileStore(function<DatabaseContext*(void)> gContext, Storage* s = nullptr);

		/// <summary>
		/// �o��Storage�|�b��l���|�U�A�[�@��Files
		/// </summary>
		Storage* GetStorage();

		ResourceStore<char*>* GetStore();

		int AddFile(FileInfo* fInfo);

		FileInfo* GetFile();

	};

}}



#endif