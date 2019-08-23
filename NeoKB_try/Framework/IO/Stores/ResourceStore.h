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

		ResourceStore(ResourceStore<T>* store = nullptr);

		ResourceStore(vector<ResourceStore<T>*>* ss);

		virtual T Get(string name);

		virtual ifstream* GetStream(string name);

		int AddExtension(string extension);

		int AddStore(ResourceStore<T>* store);

	protected:

		vector<string>* getFileNames(string name);

		/// <summary>
		/// 用來檢索整個資料夾下有哪些檔案
		/// </summary>
		static FileSystemInterface* fileSystemInterface;

		/// <summary>
		/// 用來檢索整個資料夾下有哪些檔案
		/// </summary>
		virtual int initializeFileSystemInterface();

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
