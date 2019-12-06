#ifndef RESOURCE_STORE_H
#define RESOURCE_STORE_H


#include <vector>
#include <iostream>
#include <string>
#include "../FileSystemInterface.h"
#include "../../Allocation/Hierachal/MtoObject.h"
#include "../FileNotFoundException.h"

using namespace std;
using namespace Framework::IO;
using namespace Framework::Allocation::Hierachal;


namespace Framework {
namespace IO {
namespace Stores {


	/// <summary>
	/// 計錄錄靜和附檔名，當你需要檔案的時候，只要呼叫Get就會去抓到你要的檔案，並且讀成stream給你
	/// T式檔案讀出來的形式，可能是byte array之類
	/// 結果T應該用不到
	/// </summary>
	template<typename T>
	class ResourceStore : public MtoObject{

	public:

		ResourceStore(): RegisterType("ResourceStore") {

		}


		virtual T Get(string name) = 0;

		virtual ifstream* GetStream(string name) = 0;
		
		virtual string GetFilePath(string name) = 0;

	};



}}}






#endif
