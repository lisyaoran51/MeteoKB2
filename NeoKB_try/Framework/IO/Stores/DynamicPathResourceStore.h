#ifndef DYNAMIC_PATH_RESOURCE_STORE_H
#define DYNAMIC_PATH_RESOURCE_STORE_H


#include <vector>
#include <iostream>
#include "ResourceStore.h"
#include <fstream>


using namespace std;


namespace Framework {
namespace IO {
namespace Stores {


	/// <summary>
	/// 計錄錄靜和附檔名，當你需要檔案的時候，只要呼叫Get就會去抓到你要的檔案，並且讀成stream給你
	/// T式檔案讀出來的形式，可能是byte array之類
	/// </summary>
	class DynamicPathResourceStore: public ResourceStore<char*> {

	public:

		DynamicPathResourceStore(string p);

		virtual char* Get(string name);

		virtual fstream* GetStream(string name);

		virtual string GetFilePath(string name);

	protected:


	private:

		string path;

	};



}}}






#endif
