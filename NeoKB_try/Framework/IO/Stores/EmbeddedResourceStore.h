#ifndef EMBEDDED_RESOURCE_STORE_H
#define EMBEDDED_RESOURCE_STORE_H

#include "ResourceStore.h"



namespace Framework {
namespace IO {
namespace Stores {

	/// <summary>
	/// 用內嵌的方式抓出檔案，還不知道要怎麼寫，所以先不寫
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