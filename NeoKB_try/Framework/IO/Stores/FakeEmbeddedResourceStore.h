#ifndef FAKE_EMBEDDED_RESOURCE_STORE_H
#define FAKE_EMBEDDED_RESOURCE_STORE_H

#include "EmbeddedResourceStore.h"
#include "../Storage.h"



using namespace Framework::IO;



namespace Framework {
namespace IO {
namespace Stores {

	/// <summary>
	/// 用內嵌的方式抓出檔案，還不知道要怎麼寫，所以先不寫，直接用storage做
	/// </summary>
	template<typename T>
	class 
		FakeEmbeddedResourceStore : public EmbeddedResourceStore<T> {

		Storage* storage;

	public:

		FakeEmbeddedResourceStore(Storage* s) : RegisterType("NamespacedResourceStore"){

		}

		virtual T Get(string name) {
			throw runtime_error("int FakeEmbeddedResourceStore::Get() : the fake resource store cannot turn into T.");
		}

		virtual ifstream* GetStream(string name) {
			if (storage->Exist(name))
				return storage->GetStream();
			else
				return nullptr;
		}

	};

}}}





#endif