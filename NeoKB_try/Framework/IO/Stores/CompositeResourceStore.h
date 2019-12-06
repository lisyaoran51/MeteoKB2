#ifndef COMPOSITE_RESOURCE_STORE_H
#define COMPOSITE_RESOURCE_STORE_H


#include <vector>
#include <iostream>
#include <string>
#include "ResourceStore.h"

using namespace std;
using namespace Framework::IO;


namespace Framework {
namespace IO {
namespace Stores {


	/// <summary>
	/// 計錄錄靜和附檔名，當你需要檔案的時候，只要呼叫Get就會去抓到你要的檔案，並且讀成stream給你
	/// T式檔案讀出來的形式，可能是byte array之類
	/// 結果T應該用不到
	/// </summary>
	template<typename T>
	class CompositeResourceStore : public ResourceStore<T> {

	public:

		CompositeResourceStore(ResourceStore<T>* store = nullptr): RegisterType("ResourceStore") {


			if (store != nullptr)
				stores.push_back(store);

		}

		
		CompositeResourceStore(vector<ResourceStore<T>*>* ss){

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
						T temp = stores[i]->Get(names->at(j));
						delete names;
						return temp;
					}
					catch (FileNotFoundException& e) {
						continue;
					}
				}
			}
			string message = string("CompositeResourceStore::Get : cannot found \"") + name + string("\"");

			delete names;

			throw FileNotFoundException((char*)message.c_str());

			return T();
		}

		virtual ifstream* GetStream(string name) {
			vector<string>* names = getFileNames(name);

			ifstream * tempStream = nullptr;

			for (int i = 0; i < stores.size(); i++) {
				for (int j = 0; j < names->size(); j++) {

					tempStream = stores[i]->GetStream(names->at(j));

					if (tempStream != nullptr) {
						delete names;
						return tempStream;
					}
				}
			}

			delete names;

			return nullptr;
		}

		virtual string GetFilePath(string name) {
			vector<string>* names = getFileNames(name);

			string returnValue = "";

			for (int i = 0; i < stores.size(); i++) {
				for (int j = 0; j < names->size(); j++) {

					try {
						returnValue = stores[i]->GetFilePath(names->at(j));
						delete names;
						return returnValue;
					}
					catch(){}
				}
			}
			delete names;
			return returnValue;
		}

		int AddExtension(string extension) {
			extensions.push_back(extension);
			return 0;
		}

		CompositeResourceStore<T>* AddStore(ResourceStore<T>* store) {

			stores.push_back(store);
			return this;
		}

	protected:

		virtual vector<string>* getFileNames(string name) {
			vector<string>* names = nullptr;
			if (name.find(".") != name.npos)
				return new vector<string>{ name };

			names = new vector<string>{ name };
			for (int i = 0; i < extensions.size(); i++) {
				names->push_back(name + string(".") + extensions[i]);
			}

			return names;
		}

		/* 內嵌式檔案的做法要在別的地方用，不是在這邊用
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
		*/
	private:

		vector<ResourceStore<T>*> stores;

		vector<string> extensions;

		

	};



}}}






#endif
