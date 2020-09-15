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
				// �S���
				return nullptr;
			}

			T* to = dynamic_cast<T*>(o);

			if (!to)
				throw invalid_argument("DependencyContainer::GetCache<T>(string): cast to wrong class type.");

			return to;
		}

		template<typename T>
		int Cache(T* o) {

			LOG(LogLevel::Fine) << "DependencyContainer::Cache() : cache " << o->GetTypeName() << "into dependency container.";

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

		int DeleteCache(string typeName) {
			
			map<string, MtoObject*>::iterator iter = cache.find(typeName);

			if (iter != cache.end())
				cache.erase(iter);
			else
				return -1;

			return 0;
		}

		int _DebugPrintCache() {
			LOG(LogLevel::Debug) << "DependencyContainer::_DebugPrintCache():";
			for (auto const& x : cache)
			{
				LOG(LogLevel::Debug) << "---Name = " << x.first << ", Address = " << x.second;
			}
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