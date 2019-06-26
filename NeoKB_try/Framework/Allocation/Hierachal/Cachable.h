#ifndef CACHABLE_H
#define CACHABLE_H

//#include"Loadable.h"
#include<map>
#include"HasParent.h"
#include <iostream>
#include "../Log.h"

using namespace std;
using namespace Util;

/*
* mutual including two classes:
* http://squall.cs.ntou.edu.tw/cpp/1042/labtest/test2/MutualReferences.html
*
*/



namespace Util {
namespace Hierachal{

	//class Loadable;

	/// <summary>
	/// to make a cache of the whole game for some configuration and common data
	/// </summary>
	class Cachable: public HasParent {

		map<string, MtoObject*> cache;

	public:

		Cachable();

		virtual ~Cachable() = default;

		template<typename T>
		T* GetCache(string type) {

			LOG(LogLevel::Finest) << "T* GetCache(string) : getting cache [" << type << "] from hierachy object [" << GetTypeName() << "].";

			MtoObject* o = getCache(type);

			if (!o) {
				// 沒找到

				HasParent* h = GetParent();
				if (!h)
					return NULL;

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
		MtoObject* getCache(string type);

	};

}}




#endif