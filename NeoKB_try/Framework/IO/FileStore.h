#ifndef FILE_STORE_H
#define FILE_STORE_H

#include "../Database/DatabaseStore.h"


using namespace Framework::Database;


namespace Framework {
namespace IO{

	class FileStore : public DatabaseStore {


	public:
		FileStore(function<DatabaseContext*(void)> getContext, Storage* s = nullptr);
	};

}}



#endif