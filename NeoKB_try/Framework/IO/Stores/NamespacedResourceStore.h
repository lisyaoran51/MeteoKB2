#ifndef NAMESPACED_RESOURCE_STORE_H
#define NAMESPACED_RESOURCE_STORE_H


#include <vector>
#include <iostream>
#include "CompositeResourceStore.h"

using namespace std;
using namespace std::literals::string_literals;


namespace Framework {
namespace IO {
namespace Stores {


	/// <summary>
	/// �u�x�s���|�A�������ɮ�Ū���A�nŪ�����ܭnadd�i�o��Store
	/// </summary>
	template<typename T>
	class NamespacedResourceStore : public CompositeResourceStore<T> {

	public:

		NamespacedResourceStore(ResourceStore<T>* store, string name): CompositeResourceStore<T>(store), RegisterType("NamespacedResourceStore"){
			nameSpace = name;
		}


	protected:

		virtual vector<string>* getFileNames(string name) {

			string temp = nameSpace + "/"s + name;
			
			return CompositeResourceStore<T>::getFileNames(temp);
		}

	private:

		string nameSpace;

	};




}}}






#endif
