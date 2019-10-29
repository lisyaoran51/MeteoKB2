#ifndef DEPENDENCY_CONTAINER_H
#define DEPENDENCY_CONTAINER_H


#include <map>
#include "Hierachal/MtoObject.h"
#include "../../Util/Log.h"



using namespace std;
using namespace Framework::Allocation::Hierachal;
using namespace Util;


namespace Framework {
namespace Allocation {


	class DependencyContainer {

		map<string, MtoObject*> cache;


	public:


		template<typename T>
		T* GetCache(string type) {

			LOG(LogLevel::Finest) << "T* DependencyContainer::GetCache(string) : getting cache [" << type << "] from DependencyContainer.";

			MtoObject* o = getCache(type);

			if (!o) {
				// 沒找到
				return nullptr;
			}

			T* to = dynamic_cast<T*>(o);

			if (!to)
				throw invalid_argument("DependencyContainer::GetCache<T>(string): cast to wrong class type.");

			return to;
		}

		template<typename T>
		int Cache(T* o) {
			cache[o->GetTypeName()] = o;

			return 0;
		}

		/// <summary>
		/// 當要cache的物件是多型時，cache進去的型別名稱要先改回覆類別的名稱
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
				// 找到

				MtoObject* c = cache[type];
				return c;
			}
			return nullptr;
		}

	};


}}

#endif