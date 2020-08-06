#include "StorageResourceStore.h"


using namespace Framework::IO::Stores;



StorageResourceStore::StorageResourceStore(Storage * s): RegisterType("StorageResourceStore")
{
	storage = s;
}

char * StorageResourceStore::Get(string name)
{
	throw runtime_error("StorageResourceStore::Get(): not implemented.");
	return nullptr;
}

fstream * StorageResourceStore::GetStream(string name)
{
	return storage->GetStream(name);
}

string StorageResourceStore::GetFilePath(string name)
{
	if (storage->Exist(name))
		return storage->GetUsableFilePathFor(name);
	else
		return string();
}
