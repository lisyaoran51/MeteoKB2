#include "FileStore.h"

#include "Stores/StorageResourceStore.h"


using namespace Framework::IO;




FileStore::FileStore(function<DatabaseContext*(void)> gContext, Storage * s): DatabaseStore(gContext, s->GetStorageForDirectory("Files")), RegisterType("FileStore")
{
	store = new StorageResourceStore(s);
}

Storage * FileStore::GetStorage()
{
	return storage;
}

ResourceStore<char*>* FileStore::GetStore()
{
	return store;
}

int FileStore::AddFile(FileInfo * fInfo)
{
	throw runtime_error("FileStore::AddFile(): don't use. useless.");
	return 0;
}

FileInfo * FileStore::GetFile()
{
	throw runtime_error("FileStore::AddFile(): don't use. useless.");
	return nullptr;
}
