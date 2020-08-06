#ifndef EMBEDDED_RESOURCE_STORE_H
#define EMBEDDED_RESOURCE_STORE_H

#include "ResourceStore.h"



namespace Framework {
namespace IO {
namespace Stores {

	/// <summary>
	/// �Τ��O���覡��X�ɮסA�٤����D�n���g�A�ҥH�����g
	/// </summary>
	template<typename T>
	class EmbeddedResourceStore : public ResourceStore<T> {

	public:

		EmbeddedResourceStore(string resourceNamespace) : RegisterType("EmbeddedResourceStore"){

		}

		virtual T Get(string name) {
			throw runtime_error("int NamespacedResourceStore::Get() : not implemented yet.");
			return T;
		}

		virtual fstream* GetStream(string name) {
			throw runtime_error("int NamespacedResourceStore::GetStream() : not implemented yet.");
			return nullptr;
		}

	};

}}}




#endif