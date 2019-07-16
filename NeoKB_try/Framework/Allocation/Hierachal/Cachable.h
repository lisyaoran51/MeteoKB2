#ifndef CACHABLE_H
#define CACHABLE_H

//#include"Loadable.h"
#include <map>
#include "HasParent.h"
#include <iostream>
#include "../../../Util/Log.h"
#include "../DependencyContainer.h"

using namespace std;
using namespace Util;
using namespace Framework::Allocation;

/*
* mutual including two classes:
* http://squall.cs.ntou.edu.tw/cpp/1042/labtest/test2/MutualReferences.html
*
*/



namespace Framework {
namespace Allocation {
namespace Hierachal{

	//class Loadable;

	/// <summary>
	/// to make a cache of the whole game for some configuration and common data
	/// </summary>
	class Cachable: public HasParent {

		DependencyContainer* dependencies;


	public:

		Cachable();

		virtual ~Cachable() = default;

		virtual int SetParent(HasParent* p);

		int SetDependencies(DependencyContainer* d);

		DependencyContainer* GetDependencies();

		template<typename T>
		T* GetCache(string type) {

			// TODO: 有可能沒有denpendency?

			return dependencies->GetCache<T>(type);

		}

		template<typename T>
		int Cache(T* o) {
			dependencies->Cache<T>(o);

			return 0;
		}

		/// <summary>
		/// 當要cache的物件是多型時，cache進去的型別名稱要先改回覆類別的名稱
		/// </summary>
		template<typename T>
		int Cache(T* o, string typeName) {
			dependencies->Cache<T>(o, typeName);

			return 0;
		}

	protected:

		virtual DependencyContainer* CreateLocalDependencies(DependencyContainer* parentDependencies);

	};

}}}




#endif