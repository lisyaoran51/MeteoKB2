#ifndef FILE_STORE_H
#define FILE_STORE_H

#include "../Database/DatabaseStore.h"
#include "Stores/ResourceStore.h"


using namespace Framework::Database;
using namespace Framework::IO::Stores;


namespace Framework {
namespace IO{

	/// <summary>
	/// file store只儲存在files資料夾下面的檔案
	/// 這是一個沒有用的東西，連在osu裡面都沒有用
	/// </summary>
	class FileStore : public DatabaseStore {

		ResourceStore<char*>* store;

	public:
		FileStore(function<DatabaseContext*(void)> gContext, Storage* s = nullptr);

		/// <summary>
		/// 這個Storage會在原始路徑下再加一個Files
		/// </summary>
		Storage* GetStorage();

		ResourceStore<char*>* GetStore();

		int AddFile(FileInfo* fInfo);

		FileInfo* GetFile();

	};

}}



#endif