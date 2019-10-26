#ifndef FAKE_EMBEDDED_RESOURCE_STORE_H
#define FAKE_EMBEDDED_RESOURCE_STORE_H

#include "EmbeddedResourceStore.h"
#include "../Storage.h"



using namespace Framework::IO;



namespace Framework {
namespace IO {
namespace Stores {

	/// <summary>
	/// �Τ��O���覡��X�ɮסA�٤����D�n���g�A�ҥH�����g�A������storage��
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