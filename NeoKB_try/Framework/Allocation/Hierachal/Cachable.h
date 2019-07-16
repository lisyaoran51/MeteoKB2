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

			// TODO: ���i��S��denpendency?

			return dependencies->GetCache<T>(type);

		}

		template<typename T>
		int Cache(T* o) {
			dependencies->Cache<T>(o);

			return 0;
		}

		/// <summary>
		/// ��ncache������O�h���ɡAcache�i�h�����O�W�٭n����^�����O���W��
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