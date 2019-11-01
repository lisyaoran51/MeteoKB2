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

ifstream * StorageResourceStore::GetStream(string name)
{
	return storage->GetStream(name);
}
