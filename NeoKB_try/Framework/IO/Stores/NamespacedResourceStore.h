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
	/// �u�x�s���|�A�������ɮ�Ū���A�nŪ�����ܭnadd�i�o��Store
	/// </summary>
	template<typename T>
	class NamespacedResourceStore : public ResourceStore<T> {

	public:

		NamespacedResourceStore(ResourceStore<T>* store, string name);

		/// <summary>
		/// read all bytes��k�G
		/// http://www.codecodex.com/wiki/Read_a_file_into_a_byte_array
		/// </summary>
		char* Get(string name);


	protected:

	private:

		string path;

	};




}}}






#endif
