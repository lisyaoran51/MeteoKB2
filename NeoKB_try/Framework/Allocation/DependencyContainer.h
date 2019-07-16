#ifndef DEPENDENCY_CONTAINER_H
#define DEPENDENCY_CONTAINER_H


#include <map>
#include "Hierachal\MtoObject.h"



using namespace std;
using namespace Framework::Allocation::Hierachal;


namespace Framework {
namespace Allocation {


	class DependencyContainer {

		map<string, MtoObject*> cache;


	public:


		template<typename T>
		T* GetCache(string type) {

			LOG(LogLevel::Finest) << "T* GetCache(string) : getting cache [" << type << "] from hierachy object [" << GetTypeName() << "].";

			MtoObject* o = getCache(type);

			if (!o) {
				// �S���

				HasParent* h = GetParent();
				if (!h)
					return nullptr;

				Cachable* c = Cast<Cachable>(h);
				//cout << "cast to cachable" << endl;
				return c->GetCache<T>(type);
			}

			T* to = Cast<T>(o);

			if (!to)
				throw invalid_argument("Cachable::GetCache<T>(string): cast to wrong class type.");

			return to;
		}

		template<typename T>
		int Cache(T* o) {
			cache[o->GetTypeName()] = o;

			return 0;
		}

		/// <summary>
		/// ��ncache������O�h���ɡAcache�i�h�����O�W�٭n����^�����O���W��
		/// </summary>
		template<typename T>
		int Cache(T* o, string typeName) {
			cache[typeName] = o;

			return 0;
		}

	protected:

		/// <summary>
		/// get the cache of a certain type in the whole hierachy
		/// </summary>
		MtoObject* getCache(string type) {

			if (cache.find(type) != cache.end()) {
				// ���

				MtoObject* c = cache[type];
				return c;
			}
			return nullptr;
		}

	};


}}

#endif