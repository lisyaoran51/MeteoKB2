#ifndef STORAGE_RESOURCE_STORE_H
#define STORAGE_RESOURCE_STORE_H




#include "ResourceStore.h"
#include "../Storage.h"




namespace Framework {
namespace IO {
namespace Stores {

	class StorageResourceStore : public ResourceStore<char*> {

		Storage* storage;

	public:

		StorageResourceStore(Storage* s);

		virtual char* Get(string name);

		virtual fstream* GetStream(string name);

		virtual string GetFilePath(string name);

	};

}}}




#endif