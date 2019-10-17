#ifndef RESOURCE_STORE_H
#define RESOURCE_STORE_H


#include <vector>
#include <iostream>
#include "../FileSystemInterface.h"

using namespace std;
using namespace Framework::IO;



namespace Framework {
namespace IO {
namespace Stores {


	/// <summary>
	/// 計錄錄靜和附檔名，當你需要檔案的時候，只要呼叫Get就會去抓到你要的檔案，並且讀成stream給你
	/// T式檔案讀出來的形式，可能是byte array之類
	/// </summary>
	template<typename T>
	class ResourceStore {

	public:

		ResourceStore(ResourceStore<T>* store = nullptr) {

			initializeFileSystemInterface();

			if (store != nullptr)
				stores.push_back(store);

		}

		ResourceStore(vector<ResourceStore<T>*>* ss){
			initializeFileSystemInterface();

			if (ss == nullptr)
				return;
			for (int i = 0; i < ss->size(); i++) {
				stores.push_back(ss->at(i));
			}
		}

		virtual T Get(string name) {
			vector<string>* names = getFileNames(name);

			for (int i = 0; i < stores.size(); i++) {
				for (int j = 0; j < names->size(); j++) {
					try {
						return stores[i].Get(names->at(j));
					}
					catch (FileNotFoundException& e) {
						continue;
					}
				}
			}
			string message = string("ResourceStore cannot found \"") + name + string("\"");

			delete names;

			throw FileNotFoundException(message.c_str());

			return T();
		}

		virtual ifstream* GetStream(string name) {
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

		int AddExtension(string extension) {
			extensions.push_back(extension);
			return 0;
		}

		int AddStore(ResourceStore<T>* store) {

			stores.push_back(store);
			return 0;
		}

	protected:

		vector<string>* getFileNames(string name) {
			vector<string>* names = nullptr;
			if (name.find("." != name.npos))
				return new vector<string>(name);

			names = new vector<string>();
			for (int i = 0; i < extensions.size(); i++) {
				names->push_back(name + string(".") + extensions[i]);
			}

			return names;
		}

		/// <summary>
		/// 用來檢索整個資料夾下有哪些檔案
		/// </summary>
		static FileSystemInterface* fileSystemInterface;

		/// <summary>
		/// 用來檢索整個資料夾下有哪些檔案
		/// </summary>
		virtual int initializeFileSystemInterface() {

			if (fileSystemInterface == nullptr)
				initializeFileSystemInterface = new LinuxFileSystem();
			return 0;
		}

	private:

		vector<ResourceStore<T>*> stores;

		vector<string> extensions;

		

	};

	struct FileNotFoundException : public runtime_error {

		FileNotFoundException(char* message): runtime_error(message) {}

		const char * what() const throw () {
			return runtime_error::what();
		}
	};


}}}






#endif
