#ifndef RESOURCE_STORE_H
#define RESOURCE_STORE_H


#include <vector>
#include <iostream>


using namespace std;


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

		ResourceStore(ResourceStore* store);

		ResourceStore(vector<ResourceStore*>* ss);

		T Get(string name);

		ifstream* GetStream(string name);

		int AddExtension(string extension);

	protected:

		vector<string>* getFileNames();

	private:

		vector<ResourceStore*> stores;

		vector<string> extensions;

	};




}}}






#endif
