#include "ResourceStore.h"


using namespace Framework::IO::Stores;

template<typename T>
ResourceStore<T>::ResourceStore(ResourceStore<T>* store = nullptr)
{
	initializeFileSystemInterface();

	if (store != nullptr)
		stores.push_back(store);

}

template<typename T>
ResourceStore<T>::ResourceStore(vector<ResourceStore<T>*>* ss)
{
	initializeFileSystemInterface();

	if (ss == nullptr)
		return;
	for (int i = 0; i < ss->size(); i++) {
		stores.push_back(ss->at(i));
	}
	
}

template<typename T>
T ResourceStore<T>::Get(string name)
{
	vector<string>* names = getFileNames(name);

	for (int i = 0; i < stores.size(); i++) {
		for (int j = 0; j < names->size(); j++) {
			try {
				return stores[i].Get(names->at(j));
			}
			catch(FileNotFoundException& e){
				continue;
			}
		}
	}
	string message = string("ResourceStore cannot found \"") + name + string("\"");

	delete names;

	throw FileNotFoundException(message.c_str());

	return T();
}

template<typename T>
ifstream * ResourceStore<T>::GetStream(string name)
{
	vector<string>* names = getFileNames(name);

	ifstream * tempStream = nullptr;

	for (int i = 0; i < stores.size(); i++) {
		for (int j = 0; j < names->size(); j++) {

			tempStream = stores[i].Get(names->at(j));

			if (tempStream != nullptr) {
				delete names;
				return tempStream;
			}
		}
	}

	delete names;

	return nullptr;
}

template<typename T>
int ResourceStore<T>::AddExtension(string extension)
{
	extensions.push_back(extension);
	return 0;
}

template<typename T>
int ResourceStore<T>::AddStore(ResourceStore<T>* store)
{
	stores.push_back(store);
	return 0;
}

template<typename T>
vector<string>* ResourceStore<T>::getFileNames(string name)
{
	vector<string>* names = nullptr;
	if (name.find("." != name.npos))
		return new vector<string>(name);

	names = new vector<string>();
	for (int i = 0; i < extensions.size(); i++) {
		names->push_back(name + string(".") + extensions[i]);
	}

	return names;
}

template<typename T>
int ResourceStore<T>::initializeFileSystemInterface()
{
	if (fileSystemInterface == nullptr)
		initializeFileSystemInterface = new LinuxFileSystem();
	return 0;
}
