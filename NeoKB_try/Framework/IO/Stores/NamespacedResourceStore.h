#ifndef NAMESPACED_RESOURCE_STORE_H
#define NAMESPACED_RESOURCE_STORE_H


#include <vector>
#include <iostream>
#include "ResourceStore.h"

using namespace std;


namespace Framework {
namespace IO {
namespace Stores {


	/// <summary>
	/// 只儲存路徑，不提供檔案讀取，要讀取的話要add進這個Store
	/// </summary>
	template<typename T>
	class NamespacedResourceStore : public ResourceStore<T> {

	public:

		NamespacedResourceStore(ResourceStore<T>* store, string name);

		/// <summary>
		/// read all bytes方法：
		/// http://www.codecodex.com/wiki/Read_a_file_into_a_byte_array
		/// </summary>
		char* Get(string name);


	protected:

	private:

		string path;

	};




}}}






#endif
